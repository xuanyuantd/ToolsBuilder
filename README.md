# ToolsBuilder
本库在cocos2d-x v3.x基础上开发，将Classes和Resources目录拷贝到cocos工程对应目录进行开发

1、Classes\third_party_lib  目录中为静态lib库，使用时工程配置中加入引用lib路径，头文件搜索路径。
2、Classes\common  目录为通用功能实现，使用时加入工程代码进行编译。

注：
1、Classes\third_party_lib\mysql 目录中libmySQL.dll请自行放入.exe同目录 （windows使用，暂时无法跨平台）
2、Classes\third_party_lib\boost boost库未编译，使用之前请自行编译，编译教程自行google。
