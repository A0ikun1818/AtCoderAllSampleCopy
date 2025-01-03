// ==UserScript==
// @name         AtCoder All Sample Copy
// @namespace    http://tampermonkey.net/
// @version      2025-01-03
// @description  AtCoderのテストケースをすべてコピーするボタンを配置する
// @author       A0ikun1818
// @match        https://atcoder.jp/contests/*/tasks/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=atcoder.jp
// @grant        GM_setClipboard
// ==/UserScript==

(function() {
    'use strict';
    const LIMIT_TIME = 2.0;

    // Your code here...
    const startTime = new Date().getTime();
    let ng = false;
    while(document.querySelector('pre[id]')==null){
        let nowTime = new Date().getTime();
        if(nowTime - startTime > LIMIT_TIME){
            ng = true;
            break;
        }
    }

    if(!ng){
        let copyAllButton = document.createElement('span');
        copyAllButton.id = "AllCopy";
        copyAllButton.innerHTML = "全サンプルコピー";

        let buttonClasses = ['btn','btn-default','btn-sm','btn-copy','ml-1'];
        for(const c of buttonClasses){
            copyAllButton.classList.add(c);
        }
        copyAllButton.setAttribute('data-toggle', 'tooltip');
        copyAllButton.setAttribute('data-trigger', 'manual');
        copyAllButton.setAttribute('data-original-title', 'Copied!');

        copyAllButton.addEventListener('click', {handleEvent: sampleAllCopy});
        document.querySelector('div.io-style > div.part > section > h3').appendChild(copyAllButton);
    }
    function sampleAllCopy(){
        let allSamples = document.querySelectorAll('span.lang-ja pre[id]');
        let cnt = 0;
        let sampleCnt = 1;
        let result = "";
        for(let sample of allSamples){
            if(cnt%2==0){
                result += "$ === Sample === $\n";
                result += "" + sampleCnt + "\n";
                sampleCnt++;
                result += "$ == INPUT == $\n";
            }else{
                result += "$ == OUTPUT == $\n";
            }
            // console.log(sample);
            result += sample.innerText;

            cnt++;
        }
        result += "$ === END === $\n";
        GM_setClipboard(result, "text");
    }
})();
