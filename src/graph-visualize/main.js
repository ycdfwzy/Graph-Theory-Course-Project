/**
 * Created by Gerwa on 2017/12/28.
 */

const {app, BrowserWindow} = require('electron');
const path = require('path');
const url = require('url');
require('electron-debug')({enabled: true});

let win;

app.on('ready', () => {
    win = new BrowserWindow({
        width: 1600,
        height: 900,
        icon: path.join(__dirname, 'favicon.ico')
    });
    win.loadURL(url.format({
        pathname: path.join(__dirname, 'sssp.html'),//默认打开的页面
        protocol: 'file:',
        slashes: true
    }));
    win.on('closed', () => {
        win = null;
    });
});


app.on('window-all-closed', () => {
    app.quit();
});