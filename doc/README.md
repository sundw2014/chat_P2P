### 计网大作业README
> 孙大伟 2014010782
> https://github.com/sundw2014/net2016_fakeXin

#### 各源文件作用
###### main.cpp
 程序入口，主线程，实例化其他功能模块
###### logindialog.cpp
 登录窗口，实现登录功能和下线功能
###### mainwindow.cpp
 好友列表窗口，允许用户建立新会话，并监听本地端口，处理其他用户的会话请求
###### updatefriendslistthread.cpp
 后台更新好友列表
###### sessionswindow.cpp
 聊天窗口，是所有会话tab页的容器
###### sessiontab.cpp
 会话标签页，与用户交互，显示信息和发送信息
###### selectfriendsdialog.cpp
 对话框，用于群聊添加成员
###### sessionworkerthread.cpp
 会话工作线程，维护socket，进行实际的网络通信
 
