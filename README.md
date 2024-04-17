# Chicken Invaders
- Thử nghiệm game: https://youtu.be/vwGsdCuc0U0

## Giới thiệu 
Game bắn gà với nhiều màn chơi đa dạng, điều khiển phi thuyền vượt qua các chướng ngại vật và tiêu diệt kẻ địch.

- [0. Cách tải game](#0-cách-tải-game)
- [1. Bắt đầu game](#1-bắt-đầu-game)
- [2. Các thành phần trong game](#2-các-thành-phần-trong-game)
- [3. Một vài hình ảnh trong game](#3-một-vài-hình-ảnh-trong-game)

## 0. Cách tải game
**Bước 1:** Clone repo này về hoặc chọn Code -> Download Zip

**Bước 2:** Cài codeblocks-20.03mingw

**Bước 3:** Mở file [ChickenInvaders.cbp](ChickenInvaders.cbp) và chạy game bằng phím F9

## 1. Bắt đầu game
- Click vào nút "Save the world!" để bắt đầu trò chơi.

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/menu.png)

- Chọn "Settings" để thay đổi độ khó và cài đặt âm thanh.

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/settings.png)

- Chọn "Control" để xem các phím điều khiển.

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/control.png)

- Chọn "Hall of fame" để xem bảng xếp hạng. Có thể làm mới bảng xếp hạng.

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/ranking.png)

- Nhấn nút "Quit" để thoát game.

## 2. Các thành phần trong game
- Phi thuyền(Gundam) ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/gundam.png): là phi thuyền do người chơi điều khiển, bị nổ nếu va chạm với kẻ địch hoặc đạn của kẻ địch.
- Các loại đạn, vũ khí của gundam:
  *  Blaster ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/blaster0.png): đạn có sát thương và tốc độ bay trung bình
  *  Boron ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/boron0.png): đạn có sát thương thấp nhưng có tốc độ bay cao
  *  Neutron ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/neutron0.png): đạn có sát thương cao, bù lại tốc độ bay khá chậm.
  *  Auto aim ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/bullet-auto-aim.png): đạn có sát thương rất cao, có khả năng truy đuổi mục tiêu, nhưng tốc độ bay chậm và có độ trễ giữa 2 lần bắn liên tiếp.
  *  Laser ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/laser.png): loại vũ khí đặc biệt, có sức tấn công vô cùng mạnh, nhưng chỉ có thể nhận được khi nhận hộp quà(gift) và sau khi nhận, người chơi chỉ có thể sử dụng laser trong 15 giây.
- Thiên thạch ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/rock.png): thiên thạch bay với vận tốc cao, làm nổ phi thuyền chỉ với một cú chạm.
- Gà nhỏ ![chicken](https://github.com/hainm112123/ChickenInvaders/assets/108868969/f7f26da9-18f0-4e8a-bf23-5bd25f64d0ff): loại gà nhỏ, có lượng máu thấp. Có khả năng thả trứng ![egg](https://github.com/hainm112123/ChickenInvaders/assets/108868969/124da69f-16e1-4eb4-b70a-a4b02122791b) gây sát thương lên phi thuyền của người chơi.
- Gà lớn ![chicken_big](https://github.com/hainm112123/ChickenInvaders/assets/108868969/e4a61785-5407-4f1d-9eed-12ff3a3c25c4): loại gà cỡ lớn, lượng máu khá cao. Ở màn chơi có gà lớn, người chơi cần hạ gục nó trong thời gian quy định. Có kĩ năng đặc biệt dịch chuyển(teleport) ![tele](https://github.com/hainm112123/ChickenInvaders/assets/108868969/d773ca17-e93a-4845-bc4b-dbb38e637804)
- Gà quân đội(Boss) ![boss](https://github.com/hainm112123/ChickenInvaders/assets/108868969/d999b775-c3ce-41fe-ab36-d03a0b25d1f6): loại gà có sức mạnh cao, lượng máu lớn, có vũ khí và kĩ năng đặc biệt.
  * Vũ khí: Bom hạt nhân ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/bomb.png): loại vũ khí nguy hiểm, có thể gây phá hủy phi thuyền qua một cú va chạm hoặc dư chấn từ vụ nổ.
  * Kĩ năng đặc biệt: Tên lửa ![rocket_boss](https://github.com/hainm112123/ChickenInvaders/assets/108868969/d5fc2f99-af65-4325-ba8b-91aabd2acb12): hóa thành một quả tên lửa cỡ lớn, lao về phía phi thuyền của người chơi, gây sát thương và tạo ra vùng dư chấn xung quanh.
 

## 3. Một vài hình ảnh trong game
