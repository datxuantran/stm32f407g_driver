/*
 * GPIO_Driver.c
 * Vorlage fürs Praktikum 
 */

#include <GPIO_Driver.h>

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_EN();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_EN();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_EN();
		}
	} else {
		// Implementieren Sie die Funktionalität der Clock Deaktivierung
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_DI();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_DI();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_DI();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_DI();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_DI();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_DI();
		}
	}
}

// Implementieren Sie die weiteren Schritte. Denken Sie daran die Einträge eines Registers zurückzusetzen, bevor Sie einzelne Bits setzen!
void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {
	// Configure the pin modus
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		//Konfigurieren des PIN in INPUT oder OUTPUT-Modus
		uint8_t pinNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
		uint8_t LSB = (pinNumber << 1);
		// Reset the mode register before setting individual bits
		pGPIOHandle->pGPIOx->MODE_REG &= ~(0x3 << LSB);
		pGPIOHandle->pGPIOx->MODE_REG |=
				(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << LSB);
	} else {
		// ####################################### Praktikum 3: IRQ ############################################################

		// IRQ-Modus (Muss erst im dritten Praktikum bearbeitet werden)
		// 1. Konfiguration für Trigger auf fallenende, steigende oder beide Flanken
		// configure rising or falling trigger for pin_x
		uint8_t lineNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT
				|| pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT) {
			EXTI->FTSR |= (1 << lineNumber);
		}
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT
				|| pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT) {
			EXTI->RTSR |= (1 << lineNumber);
		}

		// 2. Konfiguration des entsprechenden GPIO-Ports in SYSCFG_EXTICR
		// SYSCFG_PCLK_EN();
		uint8_t pinNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
		uint8_t x = pinNumber / 4;
		uint8_t offset = (pinNumber % 4) * 4;
		uint8_t PORT_CODE = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx); // A, B, C, D, E, F, G, H
		SYSCFG->EXTICR[x] |= (PORT_CODE << offset);

		// 3  Aktivieren des EXTI Interrupts handling in IMR-Register
//		uint8_t lineNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
		EXTI->IMR |= (1 << lineNumber);
		// ####################################### ENDE IRQ #####################################################################
	}

	// Configure the output speed
	uint8_t pinNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	uint8_t LSB = (pinNumber << 1);
	// Reset the output speed register before setting individual bits
	pGPIOHandle->pGPIOx->OUTPUT_SPEED &= ~(0x3 << LSB);
	pGPIOHandle->pGPIOx->OUTPUT_SPEED |=
			(pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << LSB);

	// Configure the pull-up/pull-down settings
	pinNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	// uint8_t LSB = (pinNumber << 1);
	// Reset the pull-up/down register before setting individual bits
	pGPIOHandle->pGPIOx->PULL_UP_DOWN &= ~(0x3 << LSB);
	pGPIOHandle->pGPIOx->PULL_UP_DOWN |=
			(pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << LSB);

	// Configure the output type
	pinNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	// Reset the output type register before setting individual bits
	pGPIOHandle->pGPIOx->OUTPUT_TYPE &= ~(1 << pinNumber);
	pGPIOHandle->pGPIOx->OUTPUT_TYPE |=
			(pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pinNumber);
}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {
	if (pGPIOx == GPIOA) {
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB) {
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC) {
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD) {
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE) {
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOF) {
		GPIOF_REG_RESET();
	} else if (pGPIOx == GPIOG) {
		GPIOG_REG_RESET();
	} else if (pGPIOx == GPIOH) {
		GPIOH_REG_RESET();
	} else if (pGPIOx == GPIOI) {
		GPIOI_REG_RESET();
	}
}

uint8_t GPIO_ReadFromInputPin(GPIO_Handle_t *pGPIO_Handle) {
	// Check if the GPIO handle pointer is not null
	if (pGPIO_Handle == NULL) {
		printf("Error: GPIO handle is null\n");
		return 0;  // or return an error code
	}
	uint8_t PinNumber = pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber;
	// Read the input value from the GPIO port
	uint16_t input = GPIO_ReadFromInputPort(pGPIO_Handle);

	// Apply a bit mask to isolate the desired pin value
	uint16_t mask = (1 << PinNumber);
	uint16_t result = input & mask;

	// Convert the result to a boolean value and return it
	return (result != 0);
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx) {
	// Check if the GPIO register pointer is not null
	if (pGPIOx == NULL) {
		printf("Error: GPIO register pointer is null\n");
		return 0;  // or return an error code
	}

	// Read the input data register and return its value
	uint16_t inputData = pGPIOx->INPUT_DATA_REG;
	return inputData;
}

void GPIO_WriteToOutputPin(GPIO_Handle_t *pGPIO_Handle, uint8_t Value) {
	// Extract the pin number from the GPIO configuration
	uint8_t pinNumber = pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber;

	// Check the input value and set the corresponding bit in the output data register
	if (Value == 1) {
		// Set the pin to high (logic level 1)
		pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG |= (1 << pinNumber);
	} else if (Value == 0) {
		// Set the pin to low (logic level 0)
		pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG &= ~(1 << pinNumber);
	} else {
		// Handle invalid input values - print an error message or return an error code
		printf("Error: Invalid value %d for GPIO pin\n", Value);
		// Alternatively, return an error code to indicate invalid input
		// return ERROR_INVALID_INPUT;
	}
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value) {
	// Check if the GPIO register pointer is not null
	if (pGPIOx == NULL) {
		printf("Error: GPIO register pointer is null\n");
		return;  // or return an error code
	}

	// Write the value to the GPIO output data register
	pGPIOx->OUTPUT_DATA_REG = Value;
}

void toggle_bit(uint16_t *value, int bit_position) {
	if (value == NULL || bit_position < 0 || bit_position >= 16) {
		// handle invalid inputs
		printf("Error: invalid inputs arguments\n");
		return;
	}
	*value ^= (1 << bit_position);
}

void GPIO_ToggleOutputPin(GPIO_Handle_t *pGPIO_Handle) {
	uint16_t output = pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG;
	// Toggle the pin specified by the GPIO_PinConfig
	int pin_number = pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber;
	toggle_bit(&pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG, pin_number);
}

// ####################################### Praktikum 3: IRQ ############################################################

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
	// Hinweis: Bedenken Sie welche ISER und ICER Register des Prozessors in dem verwendeten Mikrocontroller benutzt werden können.
	// Nicht alle Interrupts sind im Mikrocontroller aktiv. Überprüfen sie dazu das Handbuch (Reference Manual) des Mikrocontrollers.
	if (EnorDi == ENABLE) {
		//ToDo -> DONE Programmieren der entsprechenden ISERx register
		uint8_t NVIC_ISERx = IRQNumber / 32;
		uint8_t SETENAx = IRQNumber % 32;
		switch (NVIC_ISERx) {
		case 0:
			*(NVIC_ISER0) |= (1 << SETENAx);
			break;
		case 1:
			*(NVIC_ISER1) |= (1 << SETENAx);
			break;
		case 2:
			*(NVIC_ISER2) |= (1 << SETENAx);
			break;
		case 3:
			*(NVIC_ISER3) |= (1 << SETENAx);
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		}
	} else {
		//ToDo -> DONE Programmieren der entsprechenden ICERx register
		uint8_t NVIC_ICERx = IRQNumber / 32;
		uint8_t CLRENA = IRQNumber % 32;
		switch (NVIC_ICERx) {
		case 0:
			*(NVIC_ICER0) |= (1 << CLRENA);
			break;
		case 1:
			*(NVIC_ICER1) |= (1 << CLRENA);
			break;
		case 2:
			*(NVIC_ICER2) |= (1 << CLRENA);
			break;
		case 3:
			*(NVIC_ICER3) |= (1 << CLRENA);
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		}
	}
}

void GPIO_IRQHandling(uint8_t PinNumber) {
	// Abfrage und zurücksetzen des EXTI-PR bits
	if(EXTI->PR & (1 << PinNumber)){
		// clear
		EXTI->PR |= (1 << PinNumber);
	}
}

// ####################################### ENDE IRQ ###################################################################
