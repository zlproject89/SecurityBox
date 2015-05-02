## SecurityBox
### Security Cloud File Storage System.

##### System:
```json
Language:
    [1] C++11
    [2] PHP 5.4
Library: 
    [1] STL
    [2] Boost
    [3] Crypto++
    [4] Qt 5.4
Network:
    [1] Protocol: HTTP1.1.
    [2] IP Version: IPv4.
HTTP Server:
    [1] Apache2
Database:
    [1] Server: MySQL.
Time-Server: 
    [1] Host: NTP
    [2] PORT: 123
Host:
    [1] Hostname: 104.236.9.176
    [2] port_num: 9090
```

##### Security Algorithms:
```json
[1] Asymmetric encryption scheme: RSA.
[2] symmetric encryption scheme: AES(CBC).
[3] Hash: SHA-3.
[4] Digital Signature: DSA.
```

##### Security Arguments:
```json
[1] Symmetric encryption nonce: 
    (1) length = 32.
[2] Timestamp: 
    (1) format is number sequence.
    (2) time window threshold: 60 seconds.
````

##### Operation:
```json
[1] query server rsa public key
[2] signup
[3] login
[4] logout
[5] list
[6] remove
[7] rename
[8] generate file encrypt nonce
[9] query file encrypt nonce
[10] upload file
[11] download file
```

##### Plaintext package format:
###### Request json format(Client --> Server):

```json
    {
        "operation_package":"xxxxxxxx"
    }
```
###### Response json format(Server --> Client):

```json
    {
        "operation_package":"xxxxxxxx"
    }
```

##### Plaintext error code --> error message:
```json
"0": no error
"1": request error
```

##### Security package format:
###### Request json format(Client --> Server):

Asymmetric Encryption:

`Note: HMAC is generated by rsa_encrypted_package.`
```json
    {
        "HMAC":"xxxxxxx",
        "HMAC_key":"xxxxxxx",
        "signature":"xxxxxxxx",
        "DSA_public_key":"xxxxxxx",
        "rsa_encrypted_package":"xxxxxx"
    }
```

Symmetric Encryption:
```
Note: [1] HMAC is generated by (symmetric_encrypted_operation_package + 
                                symmetric_encrypted_file +
                                session_id)
      [2] If there is no file attached, symmetric_encrypted_file = "0".
      [3] "+" sign means "string append" operation, 
          there is no space between two strings during applying "string append" operation.
```
```json
    {
        "HMAC":"xxxxxx",
        "HMAC_key":"xxxxxxxx",
        "signature":"xxxxxx",
        "DSA_public_key":"xxxxxx",
        "symmetric_encrypted_operation_package":"xxxxxxxxxx",
        "symmetric_encrypted_file":"[optional]xxxxxxxxx",
        "session_id":"xxxxxxxx"
    }
```

###### Response json format(Server --> Client):

Asymmetric Encryption:
```
Note: [1] HMAC is generated by (rsa_encrypted_package + 
                                security_error_code +
                                security_error_message)
      [2] "+" sign means "string append" operation, 
          there is no space between two strings during applying "string append" operation.
```
```json
    {
        "HMAC":"xxxxxxx",
        "HMAC_key":"xxxxxx",
        "signature":"xxxxxxxx",
        "DSA_public_key":"xxxxxxx",
        "rsa_encrypted_package":"xxxxxx",
        "security_error_code":"XXX",
        "security_error_message":"xxxxxxx"
    }
```

Symmetric Encryption:
```
Note: [1] HMAC is generated by (symmetric_encrypted_operation_package + 
                                symmetric_encrypted_file +
                                security_error_code +
                                security_error_message)
      [2] If there is no file attached, symmetric_encrypted_file = "0".
      [3] "+" sign means "string append" operation, 
          there is no space between two strings during applying "string append" operation.
```
```json
    {
        "HMAC":"xxxxxxx",
        "HMAC_key":"xxxxxxxx",
        "signature":"xxxxx",
        "DSA_public_key":"xxxxxx",
        "symmetric_encrypted_operation_package":"xxxxxx",
        "symmetric_encrypted_file":"[optional]xxxxxxxx",
        "security_error_code":"XXX",
        "security_error_message":"xxxxxxx"
    }
```

##### security_error_code --> security_error_message:

* "0": no error.
* "1": HMAC does not match.
* "2": signature does not match.
* "3": decrypt error.
* "4": timestamp does not satisfy. 
* "5": unknown error.


##### Operation package format:
###### Request format(Client --> Server):
`All URL based on RESTful API Rules`

[1] query server rsa public key:`[plaintext transmission]`

    Note: This operation should be executed before all other operations.
```json
    URL: http://hostname:port_num/security_box_api/server_rsa_public_key.php
    
    JSON:
    {
        "timestamp":"xxxxxxx"
    }
```

[2] signup:`[asymmetric encryption transmission]`

    Note: The operation "signup" does not include "login" behavior,
          which means, after "signup", user should execute "login" operation to access account.
```json
    URL: http://hostname:port_num/security_box_api/signup_service.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "password_hash":"xxxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[3] login:`[asymmetric encryption transmission]`
```json
    URL: http://hostname:port_num/security_box_api/login_service.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "password_hash":"xxxxxxxx",
        "client_rsa_public_key":"xxxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[4] logout:`[symmetric encryption transmission]`
```json
    URL: http://hostname:port_num/security_box_api/logout_service.php

    JSON:
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx"
    }
```

[5] list:`[symmetric encryption transmission]`
```json
    URL: http://hostname:port_num/security_box_api/list_service.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx"
    }
```

[6] remove:`[symmetric encryption transmission]`
```json
    URL: http://hostname:port_num/security_box_api/remove_service.php

    JSON:
    {
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[7] rename:`[symmetric encryption transmission]`
```json
    URL: http://hostname:port_num/security_box_api/rename_service.php

    JSON:
    {
        "username":"xxx@xxx",
        "original_filename":"xxxxxx",
        "new_filename":"xxxxxx",
        "timestamp":"xxxxxx"
    }
```

[8] generate file encrypt nonce:`[symmetric encryption transmission]`

    Note: Before executing "upload", user should apply this operation.
```json
    URL: http://hostname:port_num/security_box_api/generate_file_encrypt_nonce_service.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxxx"
    }
```

[9] query file encrypt nonce:`[symmetric encryption transmission]`

    Note: Before executing "download", user should apply this operation.
```json
    URL: http://hostname:port_num/security_box_api/file_encrypt_nonce.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[10] upload file:`[symmetric encryption transmission]`

    Note: [1] File content is encrypted separately, 
              and file encryption symmetric private key = password_plaintext + nonce.
          [2] Before executing this operation, need execute "generate file encrypt nonce" operation.
        
```json
    URL: http://hostname:port_num/security_box_api/upload_file_service.php

    {
        "username":"xxx@xxx",
        "filename":"xxxxxx",
        "file_encrypt_nonce":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[11] download file:`[symmetric encryption transmission]`

     Note: Before executing this operation, need execute "query file encrypt nonce" operation.
```json
    URL: http://hostname:port_num/security_box_api/file.php

    {
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```



###### Response json format(Server --> Client):

[1] query server rsa public key:`[plaintext transmission]`
```json
    {
        "server_rsa_public_key":"xxxxxx",
        "timestamp":"xxxxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[2] signup:`[asymetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[3] login:`[asymmetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "session_id":"xxxxxxx",
        "symmetric_key":"xxxxxxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[4] logout:`[symmetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[5] list:`[symmetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "filename_list":[
            "xxxxxx",
            "xxxxxx",
            "xxxxxx"
        ],
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[6] remove:`[symmetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "removed_filename":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxx"
    }
```

[7] rename:`[symmetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "original_filename":"xxxxxx",
        "new_filename":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[8] generate file encrypt nonce:`[symmetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "file_encrypt_nonce":"xxxxx",
        "timestamp":"xxxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxxxx"
    }
```

[9] query file encrypt nonce:`[symmetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "file_encrypt_nonce":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxxx"
    }
```

[10] upload file:`[symmetric encryption transmission]`
```json
    {
        "username":"xxx@xxx",
        "filename":"xxxxxx",
        "filesize":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[11] download file:`[symmetric encryption transmission]`

     Note: The encrypted file content is attached separately.
```json
    {
        "username":"xxx@xxx",
        "filename":"xxxxxx",
        "filesize":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```


##### error_code(type = string) --> error_message(type = string):

* "0": no error.
* "1": server cannot answer server's rsa public key.
* "2": username conflict.
* "3": server cannot create user folder.
* "4": server cannot register user info into db.
* "5": server cannot find username.
* "6": password_hash and username do not match.
* "7": server cannot get client rsa public key.
* "8": server cannot generate symmetric key.
* "9": you have been login.
* "10": cannot logout.
* "11": server cannot retrieve filename list.
* "12": server cannot find file based on given username and filename.
* "13": server cannot remove file.
* "14": filename conflict.
* "15": server cannot rename file.
* "16": server cannot generate file encrypt nonce.
* "17": server cannot answer file encrypt nonce based on given username and filename.
* "18": unknown error.



##### Server database scheme:

[1] User:
```sql
CREATE TABLE User (
    username VARCHAR(70) NOT NULL PRIMARY KEY,
    password_hash VARCHAR(100) NOT NULL,
    signup_date DATE NOT NULL,
    signup_time TIME NOT NULL
)ENGINE=INNODB;
```

[2] Session:
```sql
CREATE TABLE Session (
    session_id VARCHAR(100) NOT NULL PRIMARY KEY,
    username VARCHAR(70) NOT NULL,
    symmetric_key VARCHAR(256) NOT NULL,
    FOREIGN KEY(username) REFERENCES User(username)
)ENGINE=INNODB;
```

[3] File:
```sql
CREATE TABLE File (
    username VARCHAR(70) NOT NULL,
    filename VARCHAR(100) NOT NULL,
    file_encrypt_nonce VARCHAR(256) NOT NULL,
    modified_date DATE NOT NULL,
    modified_time TIME NOT NULL,
    PRIMARY KEY(username, filename),
    FOREIGIN KEY(username) REFERENCES User(username)
)ENGINE=INNODB;
```