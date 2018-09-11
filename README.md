# SqliteWithICU

##编译器
VS2015 32Bit

##前置条件
1. 通过VS2015 32bit版完成ICU的编译 
2. 部署位置
   D:\appsystem\icu\include
   D:\appsystem\icu\lib
   D:\appsystem\icu\bin

##编译问题
1. 配置属性->C/C++->预处理器->预处理器定义中增加：_CRT_SECURE_NO_WARNINGS
2. 链接库：Release(icuuc.lib icutu.lib icuin.lib)
           Debug  (icuucd.lib icutud.lib icuind.lib）
3. 动态库：Release(icuuc.dll icutu.dll icuin.dll)
           Debug  (icuucd.dll icutud.dll icuind.dll）
4. 将项目配置类型改为“动态库”， 目标文件扩展名改为“dll”