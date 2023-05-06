# nvic iser[0...7] 32bit register
tổng cộng 256 interrupts
Mỗi interrupt được đánh số nhất định từ 0 đến 255
NVIC có tổng cộng 256 bit
Để kích hoạt interrupt x thì đặt bit x trong NVIC thành 1 
EXTI0 có số là 6
đặt 6th-bit trong ISER0 thành 1 

Kích hoạt interrupt số 32 thì 
đặt 0th-bit trong ISER1 thành 1 


Thiết lập nvic iser register như thế nào để kích hoạt interrupt exti0 trên cortex m4 processor?

NVIC->ISER[EXTI0_IRQn / 32] = (uint32_t)(1 << (EXTI0_IRQn % 32)); // enable EXTI0 interrupt in NVIC

EXTI0_IRQn là tên của ngắt ngoài (External Interrupt) trên kênh EXTI0 trong STM32, được định nghĩa trong file header stm32fxxx.h, với giá trị là 6.

STM32_ReferenceManual.pdf vector table 372/1747 ext0 position

# Một thanh ghi có địa chỉ là 0xE000E100U làm sao để đặt bit thứ 6 trong thanh ghi này thành 1 sử dụng ngôn ngữ lập trình c
`*((volatile uint32_t*)0xE000E100U) |= (1 << 6); // Set bit 6 to 1`

```c
#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100U)

*(NVIC_ISER0) |= (1 << SETENAx);
```




# 1. Konfiguration für Trigger auf fallenende, steigende oder beide Flanken
Làm sao để nhận ra line nào để thiết lập 

<img src="/home/dathui/Documents/04_Notes/assets/image-20230506231753213.png" alt="image-20230506231753213" style="zoom:80%;" />

mỗi chân từ 0 tới 15 được kết nối lần lượt với line 0 tới line 15

# 2. Konfiguration des entsprechenden GPIO-Ports in SYSCFG_EXTICR

muốn thiết đặt cho portD 13 thì phải làm như thế nào 

- từ số pin suy ra SYSCFG_EXTICR[x] cần phải dùng
  x = pinNumber / 4

- làm sao để biết được port nào đang được sử dụng -> chọn portD ?

  <img src="/home/dathui/Documents/04_Notes/assets/image-20230506234353886.png" alt="image-20230506234353886" style="zoom:80%;" />

# 3. Aktivieren des EXTI Interrupts handling in IMR-Register

![image-20230506235736674](/home/dathui/Documents/04_Notes/assets/image-20230506235736674.png)

line number = pin number
