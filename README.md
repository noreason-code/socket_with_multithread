# socket_with_multithread
这是一个用c++实现的socket通讯工具，其包括发送端和接收端。
接收端使用多线程支持同时处理多个发送端的连接请求，其中receiver使用普通多线程方式实现，receiver_threadpool使用线程池方式实现

### 使用方法：
<pre>"""# 编译程序 make # 启动接收端 ./receiver_threadpool 127.0.0.1 1234 # 启动发送端 ./sender 127.0.0.1 1234 hello"""</pre>
