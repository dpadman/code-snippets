#!/usr/bin/expect

spawn openssl s_client -servername 65541.ecservice.zscaler.net -tlsextdebug -connect ecservice.zscaler.net:443 -cert 66334.crt -key 66334.key
expect "Enter pass phrase for 66334.key:"
send "66334\r"
send "GET /ca/zsapi/ec/v1/policyConfig?orgId=65541&locationId=66243&version=681 HTTP/1.1\r"
send "PostmanRuntime/7.19.0\r"
send "Host: 10.66.73.123\r"
send "Connection: keep-alive\r"
send "\r"
send "\r"
expect eof
