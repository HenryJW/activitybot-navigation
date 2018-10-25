# activitybot-navigation
Autonomous navigation with Parallax ActivityBot.

**Demo video**

[![Demo video](http://oi65.tinypic.com/mbgvg4.jpg)](https://www.youtube.com/watch?v=cXYDLHrG_DY)

**Required hardware**

1 ActivityBot with Propeller board

2 LEDs

1 Ping sonar module

1 Ping mount

3 IR receivers

2 IR flashlights

**Hardware usage**

LEDs used as indicators

Sonar used for obstacle detection

2 IR flashlights + 2 receivers for obstacle detection (1 for left and 1 for right) to compensate for sonar's blind spots

Third IR receiver used for optional TV remote control navigation (currently the remote control codes are hardcoded in remote_control.c)

**Resources**

[SimpleIDE](https://learn.parallax.com/tutorials/language/propeller-c/propeller-c-set-simpleide)

[SimpleIDE GitHub repo](https://github.com/parallaxinc/SimpleIDE)

[Activity bot tutorial](https://learn.parallax.com/tutorials/robot/activitybot/propeller-c-programming-activitybot)

[Configure VS C/C++ VS Code](https://github.com/Microsoft/vscode-cpptools/blob/master/Documentation/Getting%20started%20with%20IntelliSense%20configuration.md)

**Notes**
- Use `printi()` instead of `print()` or `printf()` to significantly reduce program size
