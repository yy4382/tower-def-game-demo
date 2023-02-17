# 一个塔防小游戏

使用Qt 5.15.2开发

- 整体架构

  - 游戏窗口是一个自定义的GameControl类对象，继承于QGraphicsView
  - 敌人都是AbstractEnemy类对象，继承于QGraphicsRectItem,类成员中包含了位图对象QGraphicsPixmapItem以显示图片
  - 我方单位以继承于QGraphicsRectItem的AbstractFriendObjects为基类
  - 地图格子都是继承于QGraphicsPixmapItem,以AbstractGrid为基类
  - 其他的单位也都是QGraphicsItem的子类

- 程序从./resourse/*.txt中读取关卡和队友数据信息，目前默认读取的是testMap.txt

>本程序的美术资源、模式灵感和关卡设计均来自于手游《明日方舟》