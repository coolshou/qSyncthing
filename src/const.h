#ifndef CONST_H
#define CONST_H


#define URL  "http://127.0.0.1:8384"
#define SYNCTHING "syncthing"
#define HELP_URL_0 = "http://forum.syncthing.net";
#define HELP_URL_1 = "https://github.com/syncthing/syncthing/releases";
#define HELP_URL_2 = "http://docs.syncthing.net";
#define HELP_URL_3 = "https://github.com/syncthing/syncthing/issues";
#define HELP_URL_4 = "https://github.com/syncthing/syncthing";
#define SHORTCUTS = """<b>Quit = CTRL + Q<br>Zoom Up = CTRL + +<br>\
               Zoom Down = CTRL + -<br>Zoom Reset = CTRL + 0""";
#define HELPMSG = """<h3>SyncthinGUI</h3>Qt5 GUI for Syncthing,GPLv3+LGPLv3<hr>\
<i>KISS DRY SingleFile Async CrossDesktop CrossDistro SelfUpdating</i><br><br>\
DEV: <a href=https://github.com/coolshou/syncthingui>Coolshou</a><br>\
<br> """;
//+ getoutput(SYNCTHING + ' --version')

#define BASE_JS = """var custom_css = document.createElement("style");\
custom_css.textContent = '*{font-family:Oxygen}';\
custom_css.textContent += 'body{background-color:lightgray}';\
custom_css.textContent += '.navbar-fixed-bottom{display:none}';\
document.querySelector("head").appendChild(custom_css);""";

#endif // CONST_H
