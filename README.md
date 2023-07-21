# winkey-polybar

Note: Ugly English, use machine translatio.

注意：机翻英语。

After switching from i3bar to Polybar, I saw a lot of configuration, all of them let Polybar resident on the screen. When using i3bar, I especially liked it's hide mode. I hope my Polybar can do the same.

从 i3bar 换到 Polybar 之后，我看了很多配置，都是将 Polybar 常驻在屏幕上。在使用 i3bar 时，特别喜欢它的 hide 模式（按下修饰键时才显示出来），我希望我的 Polybar 也可以如此。

So there was this project.  The idea is to get the state of a key by getting the keyboard input, and then map and unmap the window. Get keyboard input with reference to [kdb](https://kbd-project.org/ "Linux Keyboard Tools") [showkey](https://github.com/legionus/kbd/blob/master/src/showkey.c "Showkey command").  Currently, the operation of the Polybar window is implemented by calling xdotool through the system() function (Simply put, it was to call external commands, so it had a certain security risk). I'll write it later, but not now.

于是就有了这个项目。思路是通过获取键盘输入来获取某键的状态，然后再 map 与 unmap 窗口。获取键盘输入参考了 [kbd](https://kbd-project.org/ "Linux Keyboard Tools") 的 [showkey](https://github.com/legionus/kbd/blob/master/src/showkey.c "Showkey command")。目前对 Polybar 窗口的操作是通过 system() 函数调用 xdotool 实现的（简单说就是调用外部命令，所以具有一定的安全隐患），之后会再写，但不是现在。

## Notice

The current method of getting the state of a key requires root privileges. This can lead to security issues.

目前使用的监听键盘的方法需要 root 权限，这可能导致安全问题。

## How to use

First, Clone this project. 

首先，克隆这个仓库。

Then, use `make winkey-polybar` to compile it. You will get the executable file called winkey-polybar.

然后，运行 `make winkey-polybar` 来编译它，你将会获得一个叫 winkey-polybar 的可执行文件。

Finally, write it to your i3 config or other file that will boot it up, or you want run it manually.

最后，将它写进你的 i3 配置文件或者其它可以自动启动它的文件中，或者你想手动运行它。

## At the end

You can configure it by modify winkey-polybar.c. For example, you can replace 125 to other keycode which you want at the line `#define KEYCODE 125`. You can get the keycode by using `showkey -k`. You can also modify the code after KEYDOWN_TODO and KEYUP_TODO to get the effect you want.

你可以通过修改 winkey-polybar.c 来配置它，比如你可以将 `#define KEYCODE 125` 的 125 修改成你想要的 keycode，你可以使用 `showkey -k` 获取按键的 keycode。你也可以修改它下面 KEYDOWN_TODO 和 KEYUP_TODO 的代码来达到你想要的效果。

