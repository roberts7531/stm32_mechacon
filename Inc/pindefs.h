#define SELECT_PIN GPIO_PIN_11
#define SELECT_BANK GPIOB

#define START_PIN GPIO_PIN_12
#define START_BANK GPIOB

#define SQUARE_PIN GPIO_PIN_13
#define SQUARE_BANK GPIOB

#define TRIANGLE_PIN GPIO_PIN_14
#define TRIANGLE_BANK GPIOB

#define CROSS_PIN GPIO_PIN_15
#define CROSS_BANK GPIOB

#define CIRCLE_PIN GPIO_PIN_15
#define CIRCLE_BANK GPIOC

#define R3_PIN GPIO_PIN_14
#define R3_BANK GPIOC

#define R2_PIN GPIO_PIN_6
#define R2_BANK GPIOB

#define R1_PIN GPIO_PIN_7
#define R1_BANK GPIOB

#define SHIFT_MISO_PIN GPIO_PIN_8
#define SHIFT_MISO_BANK GPIOB

#define SHIFT_LOAD_PIN GPIO_PIN_9   // /PL
#define SHIFT_LOAD_BANK GPIOB

#define SHIFT_CLK_PIN GPIO_PIN_10
#define SHIFT_CLK_BANK GPIOB

#define PS2_DATA_PIN  GPIO_PIN_4
#define PS2_DATA_BANK GPIOB

#define PS2_CMD_PIN   GPIO_PIN_5
#define PS2_CMD_BANK  GPIOB

#define PS2_CLK_PIN   GPIO_PIN_3
#define PS2_CLK_BANK  GPIOB

#define PS2_ACK_PIN   GPIO_PIN_2
#define PS2_ACK_BANK  GPIOB

#define PS2_ATT_PIN   GPIO_PIN_15
#define PS2_ATT_BANK  GPIOA


#define MECHACON_RST_PIN GPIO_PIN_6
#define MECHACON_RST_BANK GPIOC

#define PSU_EN_PIN GPIO_PIN_7
#define PSU_EN_BANK GPIOC
// Byte 4
#define BTN_SELECT     (1 << 0)
#define BTN_L3         (1 << 1)
#define BTN_R3         (1 << 2)
#define BTN_START      (1 << 3)
#define BTN_UP         (1 << 4)
#define BTN_RIGHT      (1 << 5)
#define BTN_DOWN       (1 << 6)
#define BTN_LEFT       (1 << 7)

// Byte 5
#define BTN_L2         (1 << 0)
#define BTN_R2         (1 << 1)
#define BTN_L1         (1 << 2)
#define BTN_R1         (1 << 3)
#define BTN_TRIANGLE   (1 << 4)
#define BTN_CIRCLE     (1 << 5)
#define BTN_CROSS      (1 << 6)
#define BTN_SQUARE     (1 << 7)