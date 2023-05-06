• Programmiertechniken (Makro-Definitionen, Config-Structs) einsetzen können.
• Einfachen Treiber zur Steuerung von GPIO-Ports schreiben können.
• Abläufe im Falle eines Interrupts beschreiben können.
• Konzept der Vektortabelle verstehen und erklären
• Funktionierende Konfiguration eines Interrupts mit (NVIC und EXTI) erstellen können.

# Nested Interrupt Controller NVIC

![image-20230506162125900](/home/dathui/Documents/04_Notes/assets/image-20230506162125900.png)

Điều khiển ngắt thông qua NVIC - vi điều khiển 

Có thể điều khiển cho tới 240 ngắt 

Thiết lập mức độ ưu tiên từ 0-255 

​	? Thiết lập như thế nào 

Thiết lập nhóm ngắt, các ngắt trong cùng một nhóm ngắt không ngắt lẫn nhau

? Externer Non-Maskable Interrupt (NMI) là gì

## Vector and Interrupt Table

Bảng interrupt dùng để lưu trữ: 

- tên của ngắt
- vị trí của hàm xử lý ngắt trong bộ nhớ 
- mức độ ưu tiên của ngắt 

? hàm ngắt được lưu ở đâu: flash memory or rom memory 

? stm32 bao gồm những loại bộ nhớ nào 

> <delete>Khi chương trình hoạt động, sẽ có một con trỏ tới ngăn nhớ xếp / Stack Pointer, nhiệm vụ của con trỏ này là trỏ vào vị trí của hàm trong bộ nhớ flash (hàm sẽ được thực thi)</delete>
>
> Khi chương trình hoạt động có hai con trỏ khác nhau:
>
> - stack pointer: con trỏ tới ngăn nhớ xếp, quản lí dữ liệu
> - programm counter: con trỏ tới bộ nhớ chương trình, quản lí chương trình, hàm nào được thực thi 

### Phân biệt Interrupt và System Exception 

#### System Exception NMI Non-Masble Interrupt

## Ablauf Interrupt Call, quá trình xử lý khi gọi ngắt 

- Các thanh ghi của chương trình được lưu trữ vào ngăn nhớ xếp 
- vi xử lý trung tâm chuyển sang chế độ đặc biệt - chế độ thực thi ưu tiên 
- tính toán địa chỉ của interrupt service routine / quy trình phụ vụ ngắt: IRQ-Number *4 + Offset
- ISR - quy trình phục vụ ngắt sẽ được thực thi và trở về 
- Nạp các thanh ghi đã được ghi trước đó, bao gồm cả programm counters (bộ đếm chương trình)
- Tiếp tục thực thi chương trình 

## Interrupt Priorität - Độ ưu tiên của ngắt

Độ ưu tiên 

- Nếu nhiều ngắt được kích hoạt đồng thời, ngắt nào có mức ưu tiên cao nhất sẽ được thực thi trước
- Trong ARM M4, các IRQ có mức ưu tiên cao hơn sẽ ngắt các IRQ có mức ưu tiên thấp hơn 
- Các IRQ có mức độ ưu tiên cao hơn có thể gọi các IRQ thấp hơn

Các ngắt lồng nhau

- Khi ISR quy trình phục vụ ngắt đang chạy, các IRQ thường bị che đi => có thể làm mất ngắt 
- Việc vạch mặt các IRQ trong quá trình ISR có thể khiến các IRQ tự kích hoạt 

Bộ điều khiển NVIC giữ các ngắt trong thanh ghi chờ xử lý / pending register để chúng có thể được xử lý 

> IRQ là viết tắt Interrupt Request 

## External Interrupt Controller (EXTI) - Bộ điều khiển ngắt ngoài 

> Phân biệt giữa ngắt ngoài và ngắt trong - ngắt ngoài đến từ thiết bị ngoại vi, thông qua GPIO (General-purpose Input Output) / Cổng vào ra đa năng (đa mục đích)

![image-20230506174448560](/home/dathui/Documents/04_Notes/assets/image-20230506174448560.png)

- EXTI0 bộ dồn kênh, điều khiển pin0 của các port

- Cùng một thời điểm chỉ một cổng duy nhất có thể kích họat ngắt trên pin0, tương tự cho các pin khác 
  	Chọn port thông qua cấu hình thanh ghi sysconfig SYSCFG

- Các ngắt GPIOs được điều khiển thông qua bộ dồn kênh bộ điều khiển EXTI
- Một số thành phần có ngắt riêng ví dụ RCC hoặc FLASH được kết nối trực tiếp với NVIC

### Pending Register EXTI_PR

Clear bit trong PR thông qua HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0)

![image-20230506175627038](/home/dathui/Documents/04_Notes/assets/image-20230506175627038.png)

![image-20230506175659728](/home/dathui/Documents/04_Notes/assets/image-20230506175659728.png)

![image-20230506175804285](/home/dathui/Documents/04_Notes/assets/image-20230506175804285.png)

Khi edge event tới trên dây ngắt ngoài, thì bit PRx được đặt thành 1, ngắt ngoài ở trạng thái pending / đang chờ được xử lý 

Sau khi ngắt được thực thi, phải xóa bit PRx bằng cách lập trình bit đó thành 1

---

# Interrup part 2

Interrupt-Treiber-Programmierung

- EXTI-Register zur Konfiguration
- NVIC-Konfiguration
- Einbinden einer ISR
- Unterschiede Events und Interrupts
- Beispielanwendung interner und externer Button

Lernziele:

- Interrupt-Funktionalität für einen Treiber integrieren können.
  Am Beispiel GPIO mit EXTI und NVIC Konfiguration
- Unterschiede zwischen Events und Interrupts beschreiben können
- Anwendung mit Interrupt-Funktionalität aufbauen und testen können



<img src="/home/dathui/Documents/04_Notes/assets/image-20230506192637271.png" alt="image-20230506192637271" style="zoom:80%;" />

## Các bước để kích hoạt interrupt

1. **Kích hoạt gpio ở trạng thái input** 
2. **Chọn chế độ kích hoạt interrupt cho pin x / chân x thông qua việc kích hoạt falling trigger selection register hoặc rising trigger register:** 
       kích họat rising edge, falling edge hoặc cả hai 
   	thông qua **EXTI_FTSR và EXTI_RTSR**

<img src="/home/dathui/Documents/04_Notes/assets/image-20230506192937395.png" alt="image-20230506192937395" style="zoom:80%;" />

3. **Kích họat ngắt cho pin x/chân x thông qua việc kích họat interrupt mask register cho line x** 
   	thông qua **EXTI_IMR**

<img src="/home/dathui/Documents/04_Notes/assets/image-20230506192816776.png" alt="image-20230506192816776" style="zoom:80%;" />

4. **Chọn cổng thông qua sysconfig register** 
   	**SYSCFG-EXTICR[1..4]**
   	cần kích hoạt clock thông qua **RCC_APB2ENR** - **RCC APB2 peripheral clock enable register**
   <img src="/home/dathui/Documents/04_Notes/assets/image-20230506193517597.png" alt="image-20230506193517597" style="zoom:80%;" />
5. **Kích hoạt ngắt trong NVIC nested interrupt controller** 
   các thanh ghi dùng để bật tắt ngắt là NVIC-ISER[0...7] và NIC-ICER[0...7]
6. **Tạo ra interrupt handler** 

### NVIC 

![image-20230506194409642](/home/dathui/Documents/04_Notes/assets/image-20230506194409642.png)

#### Sự khác biệt giữa MCU register và processor register

MCU là microcontroller unit, là một thiết bị điện tử cấu thành từ processor và register.
Trong khi processor là bộ xử lý, có các thanh ghi riêng.

MCU register, thanh ghi của MCU dùng để lưu địa chỉ của các thiết bị I/O
Processor register, thanh ghi của process dùng để lưu giá trị cho việc tính toán: status register 