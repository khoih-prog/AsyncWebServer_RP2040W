/****************************************************************************************************************************
  AsyncWebAuthentication_RP2040W.cpp

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license

  Version: 1.5.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      13/08/2022 Initial coding for RP2040W with CYW43439 WiFi
  ...
  1.3.0   K Hoang      10/10/2022 Fix crash when using AsyncWebSockets server
  1.3.1   K Hoang      10/10/2022 Improve robustness of AsyncWebSockets server
  1.4.0   K Hoang      20/10/2022 Add LittleFS functions such as AsyncFSWebServer
  1.4.1   K Hoang      10/11/2022 Add examples to demo how to use beginChunkedResponse() to send in chunks
  1.4.2   K Hoang      28/01/2023 Add Async_AdvancedWebServer_SendChunked_MQTT and AsyncWebServer_MQTT_RP2040W examples
  1.5.0   K Hoang      30/01/2023 Fix _catchAllHandler not working bug
 *****************************************************************************************************************************/

#if !defined(_RP2040W_AWS_LOGLEVEL_)
  #define _RP2040W_AWS_LOGLEVEL_     1
#endif

#include "AsyncWebServer_RP2040W_Debug.h"

#include "AsyncWebAuthentication_RP2040W.h"
#include <libb64/cencode.h>

// For RP2040W
#include "Crypto/md5.h"
#include "Crypto/bearssl_hash.h"
#include "Crypto/Hash.h"

/////////////////////////////////////////////////

// Basic Auth hash = base64("username:password")

bool checkBasicAuthentication(const char * hash, const char * username, const char * password)
{
  if (username == NULL || password == NULL || hash == NULL)
  {
    AWS_LOGDEBUG("checkBasicAuthentication: Fail: NULL username/password/hash");

    return false;
  }

  size_t toencodeLen = strlen(username) + strlen(password) + 1;
  size_t encodedLen = base64_encode_expected_len(toencodeLen);

  if (strlen(hash) != encodedLen)
  {
    AWS_LOGDEBUG3("checkBasicAuthentication: Fail: strlen(hash) = ", strlen(hash), " != encodedLen = ", encodedLen );

    return false;
  }

  char *toencode = new char[toencodeLen + 1];

  if (toencode == NULL)
  {
    AWS_LOGDEBUG("checkBasicAuthentication: NULL toencode");

    return false;
  }

  char *encoded = new char[base64_encode_expected_len(toencodeLen) + 1];

  if (encoded == NULL)
  {
    AWS_LOGDEBUG("checkBasicAuthentication: NULL encoded");

    delete[] toencode;

    return false;
  }

  sprintf(toencode, "%s:%s", username, password);

  if (base64_encode_chars(toencode, toencodeLen, encoded) > 0 && memcmp(hash, encoded, encodedLen) == 0)
  {
    AWS_LOGDEBUG("checkBasicAuthentication: OK");

    delete[] toencode;
    delete[] encoded;

    return true;
  }

  AWS_LOGDEBUG("checkBasicAuthentication: Failed");

  delete[] toencode;
  delete[] encoded;

  return false;
}

/////////////////////////////////////////////////

static bool getMD5(uint8_t * data, uint16_t len, char * output)
{
  //33 bytes or more

  // For RP2040W
  br_md5_context _ctx;

  uint8_t i;
  uint8_t * _buf = (uint8_t*) malloc(16);

  if (_buf == NULL)
  {
    AWS_LOGDEBUG("getMD5: Can malloc _buf");

    return false;
  }

  memset(_buf, 0x00, 16);

  // For RP2040W
  br_md5_init(&_ctx);
  br_md5_update(&_ctx, data, len);
  br_md5_out(&_ctx, _buf);

  for (i = 0; i < 16; i++)
  {
    sprintf(output + (i * 2), "%02x", _buf[i]);
  }

  free(_buf);

  AWS_LOGDEBUG("getMD5: Success");

  return true;
}

/////////////////////////////////////////////////

static String genRandomMD5()
{
  // For RP2040W
  uint32_t r = rand();

  char * out = (char*) malloc(33);

  if (out == NULL || !getMD5((uint8_t*)(&r), 4, out))
    return "";

  String res = String(out);
  free(out);

  AWS_LOGDEBUG1("genRandomMD5: res = ", res);

  return res;
}

/////////////////////////////////////////////////

static String stringMD5(const String& in)
{
  char * out = (char*) malloc(33);

  if (out == NULL || !getMD5((uint8_t*)(in.c_str()), in.length(), out))
    return "";

  String res = String(out);
  free(out);

  AWS_LOGDEBUG1("stringMD5: res = ", res);

  return res;
}

/////////////////////////////////////////////////

String generateDigestHash(const char * username, const char * password, const char * realm)
{
  if (username == NULL || password == NULL || realm == NULL)
  {
    return "";
  }

  char * out = (char*) malloc(33);
  String res = String(username);

  res.concat(":");
  res.concat(realm);
  res.concat(":");

  String in = res;

  in.concat(password);

  if (out == NULL || !getMD5((uint8_t*)(in.c_str()), in.length(), out))
    return "";

  res.concat(out);
  free(out);

  AWS_LOGDEBUG1("generateDigestHash: res = ", res);

  return res;
}

/////////////////////////////////////////////////

String requestDigestAuthentication(const char * realm)
{
  String header = "realm=\"";

  if (realm == NULL)
    header.concat("asyncesp");
  else
    header.concat(realm);

  header.concat( "\", qop=\"auth\", nonce=\"");
  header.concat(genRandomMD5());
  header.concat("\", opaque=\"");
  header.concat(genRandomMD5());
  header.concat("\"");

  AWS_LOGDEBUG1("requestDigestAuthentication: header = ", header);

  return header;
}

/////////////////////////////////////////////////

bool checkDigestAuthentication(const char * header, const char * method, const char * username, const char * password,
                               const char * realm, bool passwordIsHash, const char * nonce, const char * opaque, const char * uri)
{
  if (username == NULL || password == NULL || header == NULL || method == NULL)
  {
    AWS_LOGDEBUG("AUTH FAIL: missing required fields");

    return false;
  }

  String myHeader = String(header);
  int nextBreak = myHeader.indexOf(",");

  if (nextBreak < 0)
  {
    AWS_LOGDEBUG("AUTH FAIL: no variables");

    return false;
  }

  String myUsername = String();
  String myRealm    = String();
  String myNonce    = String();
  String myUri      = String();
  String myResponse = String();
  String myQop      = String();
  String myNc       = String();
  String myCnonce   = String();

  myHeader += ", ";

  do
  {
    String avLine = myHeader.substring(0, nextBreak);

    avLine.trim();
    myHeader = myHeader.substring(nextBreak + 1);
    nextBreak = myHeader.indexOf(",");

    int eqSign = avLine.indexOf("=");

    if (eqSign < 0)
    {
      AWS_LOGDEBUG("AUTH FAIL: no = sign");

      return false;
    }

    String varName = avLine.substring(0, eqSign);
    avLine = avLine.substring(eqSign + 1);

    if (avLine.startsWith("\""))
    {
      avLine = avLine.substring(1, avLine.length() - 1);
    }

    if (varName.equals("username"))
    {
      if (!avLine.equals(username))
      {
        AWS_LOGDEBUG("AUTH FAIL: username");

        return false;
      }

      myUsername = avLine;
    }
    else if (varName.equals("realm"))
    {
      if (realm != NULL && !avLine.equals(realm))
      {
        AWS_LOGDEBUG("AUTH FAIL: realm");

        return false;
      }

      myRealm = avLine;
    }
    else if (varName.equals("nonce"))
    {
      if (nonce != NULL && !avLine.equals(nonce))
      {
        AWS_LOGDEBUG("AUTH FAIL: nonce");

        return false;
      }

      myNonce = avLine;
    }
    else if (varName.equals("opaque"))
    {
      if (opaque != NULL && !avLine.equals(opaque))
      {
        AWS_LOGDEBUG("AUTH FAIL: opaque");

        return false;
      }
    }
    else if (varName.equals("uri"))
    {
      if (uri != NULL && !avLine.equals(uri))
      {
        AWS_LOGDEBUG("AUTH FAIL: uri");

        return false;
      }

      myUri = avLine;
    }
    else if (varName.equals("response"))
    {
      myResponse = avLine;
    }
    else if (varName.equals("qop"))
    {
      myQop = avLine;
    }
    else if (varName.equals("nc"))
    {
      myNc = avLine;
    }
    else if (varName.equals("cnonce"))
    {
      myCnonce = avLine;
    }
  } while (nextBreak > 0);

  String ha1 = (passwordIsHash) ? String(password) : stringMD5(myUsername + ":" + myRealm + ":" + String(password));
  String ha2 = String(method) + ":" + myUri;
  String response = ha1 + ":" + myNonce + ":" + myNc + ":" + myCnonce + ":" + myQop + ":" + stringMD5(ha2);

  if (myResponse.equals(stringMD5(response)))
  {
    AWS_LOGDEBUG("AUTH SUCCESS");

    return true;
  }

  AWS_LOGDEBUG("AUTH FAIL: password");

  return false;
}
