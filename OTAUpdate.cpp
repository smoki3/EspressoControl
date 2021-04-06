/*
 * OTAUpdate.c
 *
 *  Created on: Apr 6, 2021
 *      Author: smoki3
 */

#include "OTAUpdate.h"
#include "DataManager.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Update.h>

WebServer otaserver(80);

/*
 * Login page
 */

const char* loginIndex =
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32 Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
             "<td>Username:</td>"
             "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
"</script>";

/*
 * Server Index Page
 */

const char* serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";


void otaUpdate_init(){
	 /*return index page which is stored in serverIndex */
	  otaserver.on("/", HTTP_GET, []() {
	    otaserver.sendHeader("Connection", "close");
	    otaserver.send(200, "text/html", loginIndex);
	  });
	  otaserver.on("/serverIndex", HTTP_GET, []() {
	    otaserver.sendHeader("Connection", "close");
	    otaserver.send(200, "text/html", serverIndex);
	  });
	  /*handling uploading firmware file */
	  otaserver.on("/update", HTTP_POST, []() {
	    otaserver.sendHeader("Connection", "close");
	    otaserver.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
	    ESP.restart();
	  }, []() {
	    HTTPUpload& upload = otaserver.upload();
	    if (upload.status == UPLOAD_FILE_START) {
	      Serial.printf("Update: %s\n", upload.filename.c_str());
	      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
	        Update.printError(Serial);
	      }
	    } else if (upload.status == UPLOAD_FILE_WRITE) {
	      /* flashing firmware to ESP*/
	      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
	        Update.printError(Serial);
	      }
	    } else if (upload.status == UPLOAD_FILE_END) {
	      if (Update.end(true)) { //true to set the size to the current progress
	        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
	      } else {
	        Update.printError(Serial);
	      }
	    }
	  });
	  otaserver.begin();
}

void otaUpdate_update(){
	otaserver.handleClient();
}
