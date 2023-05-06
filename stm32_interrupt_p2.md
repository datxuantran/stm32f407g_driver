# Interrup part 2

EXTI 
EXTI_FTSR và EXTI_RTSR
EXTI_IMR
SYSCFG-EXTICR[1..4]
RCC_APB2ENR
NVIC

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