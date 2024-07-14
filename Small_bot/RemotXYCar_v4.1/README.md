# 水車有問題可以試這個
## feature: 
- 水車馬達有增加 cool down
- servo 上電會歸零
- 不用打密碼
## 如果還有問題可以更改以下變數
- 把 `#define MOTOR_CD 60` 的數值調小或是不要變
- 把第 93 行的 `MOTOR_CD` 乘上一個倍數，例如 `MOTOR_CD * 3`