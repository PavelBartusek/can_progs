#define UseWifiServer

#include <strings.h>
#include <WiFi.h>
#include <mcp2515.h>
#include <SPI.h>
#include "NUtils.h"
#include "NCanUtils.h"

static const char ssid[] = "tplink9";
static const char password[] = "";


MCP2515 mcp2515(5);                              // Set CS to GPIO 5
WiFiServer Server(5524);
WiFiClient client;
bool connected = false;


void SetCan_Frame(can_frame & frame, const KCanFrame & Frame)
{
  frame.can_id = Frame.Id;
  frame.can_dlc = Frame.Len;
  for (int i = 0; i < Frame.Len; i++)
    frame.data[i] = Frame.Data[i];
}

void SetCanFrame(KCanFrame & Frame, const can_frame & frame)
{
  Frame.Id = frame.can_id;
  Frame.Len = frame.can_dlc;
  for (int i = 0; i < Frame.Len; i++)
    Frame.Data[i] = frame.data[i];
}

////////////////////////////////////////////////////////////////////////////////

#define BUFFER_LEN 16

int InBufferLen = 0;
struct can_frame InBuffer[BUFFER_LEN];  // input from can-bus

int OutBufferLen = 0;
struct can_frame OutBuffer[BUFFER_LEN]; // output to can-bus

// OutBuffer ==> can-bus
void OutputService()
{
  if (OutBufferLen > 0)
  {
    mcp2515.sendMessage(OutBuffer);
    OutBufferLen--;
    for (int i = 0; i < OutBufferLen; i++)
      OutBuffer[i] = OutBuffer[i+1];    
  }
}

// can-bus ==> InBuffer
void InputService()
{
  struct can_frame frame;

  if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) 
  {
    if (InBufferLen < BUFFER_LEN)
      InBufferLen++;
    InBuffer[InBufferLen-1] = frame;
  }
}

// wifi / serial ==> OutBuffer
void InSerialService()
{
  static int Len = 0;
  static bool synch = false;
  static char Buffer[64];
  KCanFrame Frame;
  int ch = -1;

  if (!client.connected())
    return;

  do
  {
    if (!client.available())
      return;
    ch = client.read();

    if (ch < 0)
      return;

    switch (char(ch))
    {
      //case 'C':
      case 'V':
      case 'O':
      case 'S':        // config. Befehle
        synch = false;
        Len = 0;
        break;

      case 't':        // transmit 11-bit can frame
      case 'T':        // transmit 29-bit can frame
      case 'r':        // transmit 11-bit RTR can frame
      case 'R':        // transmit 29-bit RTR can frame
        synch = true;
        Len = 0;
        break;
      case '\r':
      case '\n':
      {
        Buffer[Len] = 0;
        KCan232Frame can232((char *)Buffer);
        if (can232.SetCanFrame(Frame))
        {
          struct can_frame frame;
          char msg[64];

          Frame.FormatFrame(false, msg, false);
          Serial.print("From WiFi read:");
          Serial.print(msg);

          SetCan_Frame(frame, Frame);

          if (OutBufferLen < BUFFER_LEN-2)
            OutBufferLen++;
          OutBuffer[OutBufferLen-1] = frame;
        }
        synch = false;
        Len = 0;
        break;
      }
      case 0:
        break;

      default:
        if (!NUtils::IsHexDigit(char(ch)))
        {
          synch = false;
          Len = 0;
        }
        break;
    };
    if (synch && Len < 30 && ch > 0)
    {
      Buffer[Len++] = ch;
    }
  } while (true);
}

// InBuffer ==> serial / wifi
void OutSerialService()
{
  if (InBufferLen <= 0)
    return;
 
  KCan232Frame Frame;
  KCanFrame frame;

  SetCanFrame(frame, InBuffer[0]);
  Frame.GetCanFrame(frame);
  char msg[64];

  frame.FormatFrame(false, msg, false);
  if (client.connected())
  {
    strcat(Frame.msg, "\r");
    client.print(Frame.msg);
  }
  Serial.print("To WiFi sent:  ");
  Serial.print(msg);

  InBufferLen--;
  for (int i = InBufferLen; i > 0; i--)
    InBuffer[i-1] = InBuffer[i];
}

///////////////////////////////////////////////////////////////////////////////


MCP2515::ERROR PrintError(MCP2515::ERROR error, const char * msg)
{
  if (error)
  {
    Serial.print("mcp2515.");
    Serial.print(msg);
    Serial.print(" error: ");
  }
  switch (error)
  {
    case 1: 
      Serial.println("ERROR_FAIL");
      break;
    case 2:
      Serial.println("ERROR_ALLTXBUSY");
      break;
    case 3: 
      Serial.println("ERROR_FAILINIT");
      break;
    case 4: 
      Serial.println("ERROR_FAILTX");
      break;
    case 5: 
      Serial.println("ERROR_NOMSG");
      break;
    default: break;
  }

  return error;
}

void printWl_Error(int err)
{
  char str[128];
  strcpy(str, "?");
  switch (err)
  {
    case WL_NO_SHIELD: 
      strcpy(str, "WL_NO_SHIELD");
      break;
    case WL_IDLE_STATUS:
      strcpy(str, "WL_IDLE_STATUS");
      break;
    case WL_NO_SSID_AVAIL: 
      strcpy(str, "WL_NO_SSID_AVAIL");
      break;
    case WL_SCAN_COMPLETED:
      strcpy(str, "WL_SCAN_COMPLETED");
      break;
    case WL_CONNECTED:
      strcpy(str, "WL_CONNECTED");
      break;
   case WL_CONNECT_FAILED: 
      strcpy(str, "WL_CONNECT_FAILED");
      break;
    case WL_CONNECTION_LOST:
      strcpy(str, "WL_CONNECTION_LOST");
      break;
    case WL_DISCONNECTED:
      strcpy(str, "WL_DISCONNECTED");
      break;
  }
  Serial.print(str);
}

void setup() {
  
  Serial.begin(115200); 

  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("\nMAC: ");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(mac[i],HEX);
    Serial.print(":");
  }
  Serial.println(mac[5],HEX);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  int res;
  int count = 3;
  WiFi.begin(ssid, password);
  while ((res = WiFi.waitForConnectResult()) != WL_CONNECTED)
  {
    printWl_Error(res);
    Serial.println(" - Connection Failed!");
    if (count--)
    {
      delay(1000);
      //WiFi.begin(ssid, password);
      WiFi.reconnect();
    } else {
      Serial.println("Rebooting...");
      delay(5000);
      ESP.restart();
    }
  }
 
  Server.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  mcp2515.reset();
  PrintError(mcp2515.setBitrate(CAN_20KBPS, MCP_16MHZ), "setBitrate");
  PrintError(mcp2515.setNormalMode(), "setNormalMode");    
}
  

void loop() 
{ 
  if (!client.connected())
  {
    if (connected)
      Serial.println("connection broken");
    connected = false;

    client = Server.available();
    if (client.connected())
    {
      Serial.println("connected");
      connected = true;
    }
  }

  OutputService();
  InputService();
  InSerialService();
  OutSerialService();
}



