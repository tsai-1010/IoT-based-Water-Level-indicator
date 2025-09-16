#  智慧物聯網概論與應用

| 學期 | 113學年度第1學期 | 授課教師 | 李棟村 |
| --- | --- | --- | --- |
| 學號 |B1121004|B1142017|B1121010|
| 姓名 |蔡　毅|顏建偉|吳祐霆|

**專題題目：水位感測**

功能：量測水位高度，並且同時回傳ideachain網站以及Node-Red，若水位太高則亮紅燈並發出警示

**系統方塊圖：**
<img width="1253" height="699" alt="image (10)" src="https://github.com/user-attachments/assets/9c5f15c3-48e0-46f6-bf7f-06e416f6c327" />

**技術使用：**
DSI2598+連接水位感測器、LED和蜂鳴器，利用DSI2598+開發板回傳水位資訊，若高水位則響蜂鳴器以提供警示；因為DSI2598+無法同時連接兩個MQTT伺服器，所以上傳部分使用序列化連接，一次只連接一個，上傳完數據後斷開連線，改連接另一個伺服器，依此類推。

**軟硬體設計：**

軟體：Arduino、IDEAS Chain平台、MQTT explorer、Node-Red

硬體：DSI2598+、水位感測器、紅、綠、藍色LED、蜂鳴器

**環境安裝、執行步驟：**

1.至此網址下載壓縮包並解壓縮做驅動
https://github.com/rogerclarkmelbourne/Arduino_STM32

2. 點選檔案→偏好設定→額外的開發版管理員網址貼上：https://dan.drown.org/stm32duino/package_STM32duino_index.json

<img width="788" height="481" alt="image (11)" src="https://github.com/user-attachments/assets/8c146842-173e-4ea9-8b63-9c3171267f8d" />

3.點選工具 →開發板 → 開發板管理員→安裝STM32F1xx/GD32F1xx boards

4.安裝程式庫**ArduinoJson**

草稿碼 —> 匯入程式庫 —>  管理程式庫

<img width="642" height="360" alt="image (12)" src="https://github.com/user-attachments/assets/70a1a593-7904-43c1-9754-f44c403c4911" />

5.開啟 Arduino寫入以下2個程式：

**main.ino、BC26init.h**

6.燒錄進DSI2598+，連接上水位感測、LED燈、蜂鳴器(注意腳位)，並開始設定連結至idealchain及quiz.us.kg

7.做內網穿透(frps配置)再透過ubuntu連接接MQTT

<img width="563" height="541" alt="Screenshot From 2025-01-08 18-35-31" src="https://github.com/user-attachments/assets/facd5bf7-248d-4682-9fad-de69df15d3fe" />

8.把測量的數據回傳至ideaschain及quiz.us.kg，ideaschain上可以顯示出水位高度圖表，quiz.us.kg會搭配Node-Red顯示圖表

9.雙向平台互動，當蜂鳴器響時，利用MQTT 伺服器發布和訂閱控制蜂鳴器開關(測試失敗)

原因：雙向互動根據我們的投入，一定是需要加上控制的code在BC26的文件中的，但參考idealchain官網、google及youtube，皆沒有看到有反向透過idealchains去操控DSI2598+的案例。

**實體照片、執行畫面：**

此為DSI2598+上傳至MQTT(quiz.us.kg)的畫面
<img width="953" height="1005" alt="image (14)" src="https://github.com/user-attachments/assets/663c0db1-a4d0-4249-89f6-f1eed33de7f0" />

下圖為Node-Red收到數據後顯示圖表的畫面
<img width="1920" height="990" alt="image (15)" src="https://github.com/user-attachments/assets/a973fc70-a83a-4058-a4fc-6d54c88e0adf" />

下圖為監控視窗畫面
<img width="1375" height="889" alt="image (16)" src="https://github.com/user-attachments/assets/c2e9e160-0cd7-44ee-bd2f-e9d85613a00a" />

ideaschain水位折線圖
<img width="1918" height="878" alt="image (17)" src="https://github.com/user-attachments/assets/6f2eab23-f50b-4cb4-a3e5-d715688558e9" />

下圖為實體照
![ec787656-b78a-41cb-b3aa-4d9a0ea84047](https://github.com/user-attachments/assets/1c00d877-5ddd-4ac0-89e8-c4e66ad20306)

下圖為嘗試從ideaschain傳輸控制指令到DSI2598+，MQTT explorer有接收到指令但並未成功控制DSI2598+
<img width="654" height="268" alt="image (18)" src="https://github.com/user-attachments/assets/61ea8f14-038d-4741-b4d7-6f9158e52024" />

**DEMO錄影檔youtube網址：https://youtu.be/o_1v6JTHu6Q **

**心得：**

蔡毅:

藉由製作這次期末小專題，讓我對於上課所教的智慧物聯網的操作和原理更加熟悉。製作專題過程中，常常會因為一些小細節錯誤，然後修BUG修很久，但是整體不會太困難，操作出成果也很有成就感。雖然最後雙向互動的功能沒有做成功，但是在製作過程中，也學習到很多內容，不論是課內MQTT還是課外的，我希望未來能深入學習這樣的技術，將此技術能夠更廣泛應用，為社會帶來更多便利。

吳祐霆：

這次的小專題，實在是，出乎意料的不簡單，我們先是以參考前人有做過的主題，根據我們自己的興趣去加工。我認為調整控制元件的部分是最麻煩的，因為會一不小心，刪除到其他地方控制的指令，或是要導入的地方東西沒修好…… 就是基於這點，讓我意識到了智慧物聯網真的不好做的地方在哪裡，實在是需要十分的細心。再來還有像是市面上有蠻多的類似智慧物聯網的物品，既便宜也好用，相對的，也就比較少人會DIY這類型的智慧物聯網小物，相對來說，在設計我們專題時，資料十分有限，我們能鑽研、投入的部分其實也相對有限，

顏建偉:

在這次的專題中，運用了從課程一開始以及後面學到的物聯網知識，來製作這個水位感測器專題，而在開發過程中，最具挑戰性的部分是整合硬體設備和程式碼，因為需要很理解程式碼如何跟各種硬體設備溝通，以及腳位的連接，像我們的水位感測器一開始是接在3.3V的位置，結果數據一直跑不出來，還以為是設備故障，結果是要接在5V的位置，原本是要做雙向溝通的功能，但後來試了很久依然無法找出解決辦法，最後就未成功做出來，但後來還是有做出跟別組比較不一樣的技術，就是序列化連接，這些挑戰都讓我更加理解了物聯網系統的運作原理。這次的經驗不僅增強了我的實作能力，也讓我看到了智慧物聯網在未來發展的無限可能。

**參考資料來源:**

https://iforum.ideaschain.com.tw/iforum/techmatch/solution.do?solution=39

https://iforum.ideaschain.com.tw/iforum/techmatch/solution.do?solution=19

https://www.youtube.com/watch?v=8Eu0hU2vq48

**程式碼 :**

程式碼為測試雙向控制的版本，但因最後測試失敗，如果只想使用水位控制的話，一樣能正常運作，只是會多一些無關的程式碼。
