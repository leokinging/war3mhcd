原理说明：

偶然看到[url=http://bbs.islga.org/read-htm-tid-13097.html]《检测其它玩家使用Maphack捡取不可见物品-神符的工具》[/url]

，就想做一个类似的工具。

在那篇文章里有提到

[quote](quote.md)
这就几乎全使用Japi 了，思路是【循环：{获得每个玩家当前选中的单位=》若被选中单位数量>1则不是作弊；=1则=》判断此单位对此玩家是否可见=》不可

见就是此玩家使用Maphck}】

可以使用“GroupEnumUnitsSelected”这些Japi编写，把这些代码写入War3，最后可以给这些代码远程创建一个线程，也可以钩到 War3中常被执行的代码中去

。

[/quote]

首先考虑获得当前玩家的选中单位。使用GroupEnumUnitsSelected等函数是不可行的，因为单方面涉及到单位组的操作会导致掉线。用逆向思维想了一下，其实可以枚举每个单位判断是否被选中(IsUnitSelected)。

如何枚举单位呢，起初我想通过扫描内存的方式，觉得太麻烦而且耗时。我想了一个委曲求全的办法，只适用与rpg地图，现在常规对战应该玩的人不多。通过 HOOK Game.dll中对变量赋值的代码，得到变量的值，再判断是不是单位，因为rpg地图要运作就要存储单位的一些信息。

那么判断一个HANDLE是否为单位，[b](b.md)感谢Renee[/b]的启发，调用得到单位类型的函数，能得到的就是单位。在我实际实现的时候我偷懒了改成判断英雄等级，有等级的是英雄，只得到玩家的英雄信息，因为毕竟是rpg地图嘛，开图的要获得一些情报肯定要选定英雄。这段代码可以用 CheatEngine找到。

再说下判断是否在游戏中和得到玩家HANDLE的问题。判断在游戏中，地址0x6facd44c不为0就在游戏中。得到玩家HANDLE调用Player 函数就行了，但是等人进的那段时间地址0x6facd44c不为0，而且得到的玩家HANDLE和实际游戏中不一样，我就在得到英雄HANDLE之后在获得玩家信息。

到此为止似乎已经没什么问题了，开个线程每隔0.1秒（War3发包的间隔就是0.1秒）检测是否在游戏中，在的话就枚举每个玩家每个英雄是否被选中和拥有视野，如果选中却没有视野就说明开图。

但是问题又来了。第一个是调用JAPI时会崩溃，跟踪了一下发现J函数有调用TlsGetValue，使用了线程局部存储里面存的地址，所以主线程调用没事，别的线程调用就取不出地址。所以还要在HOOK TlsSetValue，判断下是否是主线程调用的，是的话把值存起来，在调用JAPI的时候用TlsSetValue赋下值。第二个是在实现后测试中发现的问题，选中一个敌方单位后这个单位走入迷雾中，虽然状态栏黑掉了，但是用IsUnitSelected判断还是选中状态，这个就误伤了没开图的同学。为了解决这个问题，我就在程序里使用GetTickCount记下每个单位对于每个玩家的最后一次选中时间和最后一次出现在视野内的时间，只有当前时间与这两者的时间都超过1秒，才做出响应。