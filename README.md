# winkey-polybar

**Note: This project has permission issues that could lead to serious consequences.**

**注意：这个项目有权限问题，可能导致严重的后果。**

Note: Ugly English, use machine translatio.

注意：机翻英语。

After switching from i3bar to Polybar, I saw a lot of configuration, all of them let Polybar resident on the screen. When using i3bar, I especially liked it's hide mode. I hope my Polybar can do the same.

从 i3bar 换到 Polybar 之后，我看了很多配置，都是将 Polybar 常驻在屏幕上。在使用 i3bar 时，特别喜欢它的 hide 模式（按下修饰键时才显示出来），我希望我的 Polybar 也可以如此。

So there was this project.  The idea is to get the state of a key by getting the keyboard input, and then map and unmap the window. Get keyboard input referenced the [showkey](https://github.com/legionus/kbd/blob/master/src/showkey.c "Showkey command") command from the [kdb](https://kbd-project.org/ "Linux Keyboard Tools") project. The window action referenced the `search`, `windowmap` and `windowunmap` command from the [xdotool](https://www.semicomplete.com/projects/xdotool/ "xdotool - fake keyboard/mouse input, window management, and more") project.

于是就有了这个项目。思路是通过获取键盘输入来获取某键的状态，然后再 map 与 unmap 窗口。获取键盘输入~~照抄~~参考了 [kbd](https://kbd-project.org/ "Linux Keyboard Tools") 的 [showkey](https://github.com/legionus/kbd/blob/master/src/showkey.c "Showkey command")，窗口操作~~照抄~~参考了 [xdotool](https://www.semicomplete.com/projects/xdotool/ "xdotool - fake keyboard/mouse input, window management, and more") 的 `search`, `windowmap` 和 `windowunmap` 命令。

## Warning

The current method of getting the state of a key requires root privileges. This can lead to security issues.

目前使用的监听键盘的方法需要 root 权限，这可能导致安全问题。

## Require

winkey-polybar uses make as its building system, and it also needs gcc (or you want to use clang, but i don't know how to use) and libx11.

winkey-polybar 使用 make 作为它的编译系统，并且还需要 gcc（或者你想用 clang，但我不知道怎么做）和 libx11。

About libx11, on Arch, you can find a package called libx11 from the package manager (When you need this project, you are already using X Window, so you should already have this package installed).

关于 libx11，在 Arch 上，你可以直接在包管理器中找到 libx11（当你需要使用这个项目的时候，说明你已经在用 X Window 了，所以这个包你应该已经装上了）。

For debian (such as Ubuntu), there is a package called libx11-dev in the package manager.

对于 Debian 系（比如 Ubuntu），有一个 libx11-dev 包在包管理器中。

For Fedora (such as RHEL), there is a package called libXt-devel in the package manager.

对于 Fedora 系（比如 RHEL），有一个 libXt-devel 包在包管理器中。

## How to use

First, Clone this project. 

首先，克隆这个仓库。

Then, use `make winkey-polybar` to compile it. You will get the executable file called winkey-polybar.

然后，运行 `make winkey-polybar` 来编译它，你将会获得一个叫 winkey-polybar 的可执行文件。

Finally, write it to your i3 config or other file that will boot it up, or you want run it manually.

最后，将它写进你的 i3 配置文件或者其它可以自动启动它的文件中，或者你想手动运行它。

## Notice

Different from before, you can now modify the default configuration by passing in KEYCODE and REGEX parameters during make. For example, `make KEYCODE=56 REGEX="eww-.*"` will make it manage the display of eww window by alt key (Maybe your alt key's keycode isn't 56?).

不同于之前，现在你可以通过在 make 时传入 KEYCODE 和 REGEX 参数来修改默认配置，比如 `make KEYCODE=56 REGEX="eww-.*"`，这会使它通过 alt 键管理 eww 窗口的显示（或许你的 alt 键的 keycode 不是 56）。

You can get the keycode by using `showkey -k`. For REGEX, since it replaces the location in the source code directly, a translation is performed when you use `"` and `\`. Also note that regular expressions require the posix ere standard.

你可以使用`showkey -k` 获取按键的 keycode，对于 REGEX，由于它会直接替换源码中的位置，所以当使用 `"` 和 `\` 时需要进行一次转译。并且注意，正则表达式需要使用 posix ere 标准。

I highly recommend to use `xdotool search --class "<your_regex>"` to test your regex expression if you want to change it in order to avoid unexpected results.

如果你要用自己的正则表达式，我非常建议在编译前使用 `xdotool search --class "<your_regex>"` 来测试你的表达式，以免出现不符合预期的结果。

