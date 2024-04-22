# Chicken Invaders
- Thử nghiệm game: [https://youtu.be/vwGsdCuc0U0](https://www.youtube.com/watch?v=gKGcTRLXTNg)

## Giới thiệu 
Game bắn gà với nhiều màn chơi đa dạng, điều khiển phi thuyền vượt qua các chướng ngại vật và tiêu diệt kẻ địch.

- [0. Cách tải game](#0-cách-tải-game)
- [1. Bắt đầu game](#1-bắt-đầu-game)
- [2. Các thành phần trong game](#2-các-thành-phần-trong-game)
  * [a. Người chơi](#a-người-chơi)
  * [b. Kẻ địch](#b-kẻ-địch)
  * [c. Khác](#c-khác)
- [3. Điều khiển và gameplay](#3-điều-khiển-và-gameplay)
- [4. Một vài hình ảnh trong game](#4-một-vài-hình-ảnh-trong-game)
- [5. Về source code của game](#5-về-source-code-của-game)

## 0. Cách tải game
**Bước 1:** Clone repo này về hoặc chọn Code -> Download Zip

**Bước 2:** Cài codeblocks-20.03mingw

**Bước 3:** Mở file [ChickenInvaders.cbp](ChickenInvaders.cbp) và chạy game bằng phím F9

## 1. Bắt đầu game

- Màn hình chính:

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/menu.png)

- Click vào nút "Save the world!" để chọn chế độ chơi. Có 2 chế độ:
  * Single player: 1 người chơi
  * Multiplayer: 2 người chơi

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/play.png)

- Chọn "Settings" để thay đổi độ khó và cài đặt âm thanh.
  * Có 4 độ khó: Easy, Normal, Hard, Insane
  * Âm thanh: có thể bật, tắt âm thanh, thay đổi độ lớn của nhạc nền(BGM) và âm thanh hiệu ứng(SFX)

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/settings.png)

- Chọn "Control" để xem các phím điều khiển.

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/control.png)

- Chọn "Hall of fame" để xem bảng xếp hạng. Có thể làm mới bảng xếp hạng bằng nút "Reset ranking"

![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/ranking.png)

- Nhấn nút "Quit" để thoát game.

## 2. Các thành phần trong game
### a. Người chơi
- Phi thuyền(Gundam) ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/gundam.png): là phi thuyền do người chơi điều khiển, bị nổ nếu va chạm với kẻ địch hoặc đạn của kẻ địch.
- Lá chắn ![sheild](https://github.com/hainm112123/ChickenInvaders/assets/108868969/44399e6c-b8bb-42c6-a245-7d3f3b0c6a95): Sau khi hồi sinh, phi thuyền sẽ nhận được một lá chắn có khả năng chặn mọi loại sát thương trong một khoảng thời gian ngắn.
- Các loại đạn, vũ khí của gundam:
  *  Blaster ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/blaster0.png): đạn có sát thương và tốc độ bay trung bình
  *  Boron ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/boron0.png): đạn có sát thương thấp nhưng có tốc độ bay cao
  *  Neutron ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/neutron0.png): đạn có sát thương cao, bù lại tốc độ bay khá chậm.
  *  Auto aim ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/bullet-auto-aim.png): đạn có sát thương rất cao, có khả năng truy đuổi mục tiêu, nhưng tốc độ bay chậm và có độ trễ giữa 2 lần bắn liên tiếp.
  *  Laser ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/laser.png): loại vũ khí đặc biệt, có sức tấn công vô cùng mạnh, nhưng chỉ có thể nhận được khi nhận hộp quà(gift) và sau khi nhận, người chơi chỉ có thể sử dụng laser trong 15 giây.
  *  Tên lửa: ![rocket](https://github.com/hainm112123/ChickenInvaders/assets/108868969/38f95334-d128-4133-9566-52c57530a788): Gây một lượng sát thương lớn lên toàn bộ kẻ địch, đồng thời phá hủy đạn pháo của chúng. Có thể nhận được khi nhặt đùi gà.

- Tăng cấp ![level_up](https://github.com/hainm112123/ChickenInvaders/assets/108868969/556ddd32-3d17-4db1-8ba4-77841ba5de67): Nhặt sẽ tăng cấp cho phi thuyền, nâng cấp sức mạnh của các loại đạn
- Hộp quà(Gift): Cung cấp cho phi thuyền loại vũ khí mới hoặc nâng cấp nếu đã có loại vũ khí đó. Các loại hộp quà tương ứng với các loại vũ khí
  * Blaster: ![gift0](https://github.com/hainm112123/ChickenInvaders/assets/108868969/3086c019-99aa-44c4-a74e-88c3a8fbcebc)
  * Boron: ![gift1](https://github.com/hainm112123/ChickenInvaders/assets/108868969/d872c30f-efe5-486d-9157-d8707888146e)
  * Neutron: ![gift2](https://github.com/hainm112123/ChickenInvaders/assets/108868969/27813d4e-d1f7-470f-be24-f39439e8b67a)
  * Auto aim: ![gift3](https://github.com/hainm112123/ChickenInvaders/assets/108868969/41d997b6-c88a-4dd6-a5ea-58a0c2167eb3)
  * Laser: ![gift_laser](https://github.com/hainm112123/ChickenInvaders/assets/108868969/46e55ac7-b61e-4112-a3a4-bd214bb2d36d)

- Thông tin của phi thuyền: ![information](https://github.com/hainm112123/ChickenInvaders/assets/108868969/0d3bda30-0ba8-468c-919f-5f457e976e52)
  * Mạng: Mỗi lần phi thuyền nổ (do va chạm với kẻ địch hạn đạn pháo của kẻ địch), phi thuyền sẽ mất 1 mạng. Người chơi sẽ thua nếu phi thuyền mất hết mạng.
  * Cấp độ: Sức mạnh đạn do phi thuyền bắn ra phụ thuộc vào cấp độ. Tăng cấp sẽ nâng cấp sát thương và hình dạng của đạn
  * Số đùi gà phi thuyền đã nhặt: Mỗi 30 đùi gà nhặt được sẽ đổi ra một lần sử dụng tên lửa
  * Số lần sử dụng tên lửa

### b. Kẻ địch
- Thiên thạch ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/rock.png): thiên thạch bay với vận tốc cao, làm nổ phi thuyền chỉ với một cú chạm.
- Gà nhỏ ![chicken](https://github.com/hainm112123/ChickenInvaders/assets/108868969/f7f26da9-18f0-4e8a-bf23-5bd25f64d0ff): loại gà nhỏ, có lượng máu thấp. Có khả năng thả trứng ![egg](https://github.com/hainm112123/ChickenInvaders/assets/108868969/124da69f-16e1-4eb4-b70a-a4b02122791b) gây sát thương lên phi thuyền của người chơi.
- Gà lớn ![chicken_big](https://github.com/hainm112123/ChickenInvaders/assets/108868969/e4a61785-5407-4f1d-9eed-12ff3a3c25c4): loại gà cỡ lớn, lượng máu khá cao. Ở màn chơi có gà lớn, người chơi cần hạ gục nó trong thời gian quy định. Có kĩ năng đặc biệt dịch chuyển(teleport) ![tele](https://github.com/hainm112123/ChickenInvaders/assets/108868969/d773ca17-e93a-4845-bc4b-dbb38e637804)
- Gà quân đội(Boss) ![boss](https://github.com/hainm112123/ChickenInvaders/assets/108868969/d999b775-c3ce-41fe-ab36-d03a0b25d1f6): loại gà có sức mạnh cao, lượng máu lớn, có vũ khí và kĩ năng đặc biệt.
  * Vũ khí: Bom hạt nhân ![image](https://github.com/hainm112123/ChickenInvaders/blob/master/assets/images/bomb.png): loại vũ khí nguy hiểm, có thể gây phá hủy phi thuyền qua một cú va chạm hoặc dư chấn từ vụ nổ.
  * Kĩ năng đặc biệt 1: Tên lửa ![rocket_boss](https://github.com/hainm112123/ChickenInvaders/assets/108868969/d5fc2f99-af65-4325-ba8b-91aabd2acb12): hóa thành một quả tên lửa cỡ lớn, lao về phía phi thuyền của người chơi, gây sát thương và tạo ra vùng dư chấn xung quanh.
  * Kĩ năng đặc biệt 2: Laser ![boss_skill_laser](https://github.com/hainm112123/ChickenInvaders/assets/108868969/497f9073-aa5a-43c1-b8ff-5bc92c56e2a0): Tạo ra những trùm laser có sức sát thương cực lớn.
- Thanh máu ![boss_health_bar](https://github.com/hainm112123/ChickenInvaders/assets/108868969/4f26eccf-0169-4202-a460-1be026f8cf87): Thanh máu của kẻ địch, hiện khi đối đầu với gà lớn hoặc gà quân đội.
- Đùi gà ![fried-chicken](https://github.com/hainm112123/ChickenInvaders/assets/108868969/a21bf939-8e05-47a3-bcd0-a3e586183072): Rơi ra khi tiêu diệt kẻ địch. Nhặt được một số lượng nhất định sẽ cho người chơi một quả tên lửa.

### c. Khác
- Nút tạm dừng (pause) ![pause_button](https://github.com/hainm112123/ChickenInvaders/assets/108868969/4be8c34b-ae71-4d8b-b481-e984360b2a4d): Click nút này sẽ tạm dừng trò chơi. Có thể sử dụng phím ESC thay thế. Sau khi nhấn, hiện lên màn hình chờ như hình dưới, bao gồm các lựa chọn:
  * Tiếp tục trò chơi
  * Quay về nàn hình chính
  * Bật/Tắt âm thanh.
![game_pause](https://github.com/hainm112123/ChickenInvaders/assets/108868969/fb76cfd8-0dfd-4d3c-839d-7c00c383d2fc)

## 3. Điều khiển và gameplay

- Các phím điều khiển:
  * Di chuyển: các phím W, A, S, D tương ứng với di chuyển lên, sang trái, xuống, sang phải
  * Đổi vũ khí: Q
  * Sử dụng tên lửa: R
  * Bắn đạn: Space
  * ESC: tạm dừng game
- Về gameplay
  * Trò chơi sẽ diễn ra theo từng màn, nhiệm vụ của người chơi là sống sót, tiêu diệt kẻ địch, vượt qua các màn và đạt điểm số cao nhất có thể
  * Có nhiều độ khó cho người chơi lựa chọn. Độ khó cao hơn cũng sẽ cho điểm cao hơn khi tiêu diệt kẻ địch và qua màn
  * Có 2 chế độ là 1 người chơi và 2 người chơi
  * Trò chơi sẽ kết thúc khi phi thuyền của người chơi hết mạng
  * Sau khi trò chơi kết thúc, một màn hình để người chơi nhập tên lên bảng xếp hạng sẽ xuất hiện
    ![game_over](https://github.com/hainm112123/ChickenInvaders/assets/108868969/86ef7882-d670-403e-b425-347dae8e7d8e)

## 4. Một vài hình ảnh trong game

![round_chicken](https://github.com/hainm112123/ChickenInvaders/assets/108868969/5a755b37-c000-458e-a79f-30953ec5918d)
![round_chicken_cycle](https://github.com/hainm112123/ChickenInvaders/assets/108868969/e9f50c16-e7aa-4a47-b9d5-17c2a911cae8)
![round_chicken_dodge](https://github.com/hainm112123/ChickenInvaders/assets/108868969/f64d8949-871b-4bf5-ad6c-4e3ed7a89013)
![round_rock_fall](https://github.com/hainm112123/ChickenInvaders/assets/108868969/fb442eea-74da-40ee-a806-688cc215f3e5)
![round_rock_side](https://github.com/hainm112123/ChickenInvaders/assets/108868969/9bffa0f9-95e5-4f77-b07a-f67c7d01c593)
![boss_rocket](https://github.com/hainm112123/ChickenInvaders/assets/108868969/cce504fa-8c84-456a-adab-004aa1d8ffc9)
![boss_laser](https://github.com/hainm112123/ChickenInvaders/assets/108868969/a4d00983-e645-4d7f-b88f-5638a9f05e0e)
![game_laser](https://github.com/hainm112123/ChickenInvaders/assets/108868969/0fe592e6-453e-41cb-9e68-348ce1471731)
![multiplayer](https://github.com/hainm112123/ChickenInvaders/assets/108868969/66b61ac6-eaf5-45b8-bb33-0581dcbc7ff7)


## 5. Về source code của game
- Folder assets: Chứa các folder hình ảnh, âm thanh, phông chữ, dữ liệu của game
- Folder common: chứa các file được sử dụng chung, nhiều bởi các file khác
  * common: chứa các hàm chung
  * entity: class đại diện cho các đối tượng trong game
  * time: class quản lí thời gian và class tính thời gian
- Folder game: 
  * game: quản lí tất cả các hoạt động, sự kiện trong game
  * media: quản lí hình ảnh và âm thanh
  * text: quản lí phần chữ hiển thị trong game
  * slider: thanh trượt, dùng cho phần cài đặt
- Folder gundam:
  * gundam: quản lí các hoạt động và sự kiện của phi thuyền
  * upgrade: các nâng cấp bao gồm tăng cấp và các gói quà
- Folder weapon:
  * bullet: quản lí các loại đạn, của cả phi thuyền và kẻ địch
  * rocket: các xử lí liên quan đến tên lửa
- Folder enemy:
  * chicken: chứa tất cả các loại gà
  * fried_chicken: quản lí hoạt động của đùi gà, thứ rơi ra khi tiêu diệt gà
  * rock: chứa các loại thiên thạch
- ChickenInvaders.cbp: project file
- main.cpp: Hàm main của trò chơi
- sdl_init.cpp và sdl_init.h: Khởi tạo SDL
