# Panda Wrapper Documentation
### *By Panda Technologies*

[![made-with-Markdown](https://img.shields.io/badge/Made%20with-Markdown-1f425f.svg)](http://commonmark.org)
![C#](https://img.shields.io/badge/Made%20With-C%23-blue)
![C++](https://img.shields.io/badge/Made%20With-C%2B%2B-blue)

#### [*Click Here For TXT Documentation*](https://pastebin.com/YwA3JJx2)
---
### Hosted+
#### How To Add Hosted+ To Your Project:

1. Include the files in your project directory, and inside the project.
2. Project Properties -> VC++ Directories -> Library Directories

`
$(MsBuildProjectDirectory)\Hosted+\cURL
`

`
$(MsBuildProjectDirectory)\Hosted+\JSON
`

3. Project Properties -> Linker -> Input

`libcurl_a.lib`

`jsoncpp.lib`

`Ws2_32.lib`

`Wldap32.lib`

`crypt32.lib`

`Normaliz.lib`

4. Project Properties -> Preprocessor ->

`
CURL_STATICLIB
`

5. Apply then your good to go.
---
### How To Use Hosted+ 

To Get Started, First Do:
```
HostedPlus::run();
```

To Get The Address You Can Do:
```
HostedPlus::json["addresses"]["addr name"]["addr"]
```

To Get The Conv You Can Do:
```
HostedPlus::json["addresses"]["addr name"]["cc"]
```

To Add A Function You Can Do:
```
auto function = HostedPlus::CreateDef<typedef,args...>("addr name");
```
