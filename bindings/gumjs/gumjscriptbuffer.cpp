/*
 * Copyright (C) 2015 Ole André Vadla Ravnås <oleavr@nowsecure.com>
 *
 * Licence: wxWindows Library Licence, Version 3.1
 */

#include "gumjscriptcore.h"

#include "gumjscript-priv.h"
#include "gumjscriptvalue.h"

#include "wtf/Platform.h"
#include "JSExportMacros.h"
#include "JSArrayBuffer.h"
#include "APICast.h"

using namespace JSC;

gpointer
_gumjs_array_buffer_get_data (GumScriptCore * core,
                              JSValueRef value,
                              gsize * size)
{
  gpointer data;
  JSValueRef exception;

  if (!_gumjs_array_buffer_try_get_data (core, value, &data, size, &exception))
    _gumjs_panic (core->ctx, exception);

  return data;
}

gboolean
_gumjs_array_buffer_try_get_data (GumScriptCore * core,
                                  JSValueRef value,
                                  gpointer * data,
                                  gsize * size,
                                  JSValueRef * exception)
{
  JSContextRef ctx = core->ctx;
  ExecState * exec = toJS (ctx);
  JSLockHolder lock (exec);

  JSValue jsValue = toJS (exec, value);
  ArrayBuffer * buffer = toArrayBuffer (jsValue);
  if (buffer != NULL)
  {
    *data = buffer->data ();
    if (size != NULL)
      *size = buffer->byteLength ();
    return TRUE;
  }
  else
  {
    _gumjs_throw (core->ctx, exception, "expected an ArrayBuffer");
    return FALSE;
  }
}