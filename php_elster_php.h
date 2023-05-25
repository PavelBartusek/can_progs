/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * This file is not intended to be easily readable and contains a number of
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG
 * interface file instead.
 * ----------------------------------------------------------------------------- */

#ifndef PHP_ELSTER_PHP_H
#define PHP_ELSTER_PHP_H

extern zend_module_entry elster_php_module_entry;
#define phpext_elster_php_ptr &elster_php_module_entry

#ifdef PHP_WIN32
# define PHP_ELSTER_PHP_API __declspec(dllexport)
#else
# define PHP_ELSTER_PHP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(elster_php);
PHP_MSHUTDOWN_FUNCTION(elster_php);
PHP_RINIT_FUNCTION(elster_php);
PHP_RSHUTDOWN_FUNCTION(elster_php);
PHP_MINFO_FUNCTION(elster_php);

ZEND_NAMED_FUNCTION(_wrap_elster_geterrormsg);
ZEND_NAMED_FUNCTION(_wrap_elster_getvaluemsg);
ZEND_NAMED_FUNCTION(_wrap_elster_getvalue);
ZEND_NAMED_FUNCTION(_wrap_elster_setvalue);
ZEND_NAMED_FUNCTION(_wrap_elster_setbits);
ZEND_NAMED_FUNCTION(_wrap_elster_clrbits);
ZEND_NAMED_FUNCTION(_wrap_elster_getstring);
ZEND_NAMED_FUNCTION(_wrap_elster_setstring);
ZEND_NAMED_FUNCTION(_wrap_elster_getname);
ZEND_NAMED_FUNCTION(_wrap_elster_gettype);
ZEND_NAMED_FUNCTION(_wrap_elster_toggle_trace);
ZEND_NAMED_FUNCTION(_wrap_elster_setdev);
ZEND_NAMED_FUNCTION(_wrap_elster_setcs);
ZEND_NAMED_FUNCTION(_wrap_elster_set_can232);
ZEND_NAMED_FUNCTION(_wrap_elster_initcan);
ZEND_NAMED_FUNCTION(_wrap_elster_undef);
ZEND_NAMED_FUNCTION(_wrap_elster_setsniffedframe);
ZEND_NAMED_FUNCTION(_wrap_elster_getsniffedvalue);
ZEND_NAMED_FUNCTION(_wrap_new_elster);
#endif /* PHP_ELSTER_PHP_H */
