/**
 * Created by Gerwa on 2017/12/30.
 */
const electron = require('electron');
const nprogress = require('nprogress');
const $ = require('jquery');
const d3 = require('d3');
const libcpp = require('./export');
const cpp = new libcpp();
const shared = {
    working: false
};