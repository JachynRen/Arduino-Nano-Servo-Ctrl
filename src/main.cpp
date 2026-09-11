/*
 * Arduino Nano Servo Control Example
 *
 * 使用 Servo 库直接通过引脚控制舵机
 *
 * 舵机信号线连接 (10个舵机):
 *   舵机 0 -> D2     舵机 1 -> D3     舵机 2 -> D4     舵机 3 -> D5
 *   舵机 4 -> D6     舵机 5 -> D7     舵机 6 -> D8     舵机 7 -> D9
 *   舵机 8 -> D10    舵机 9 -> D11
 *
 * 注意: D0/D1 用于串口通信，不用于舵机。
 *
 * 串口指令:
 *   S<编号>:<角度>    例: S0:90    舵机0转到90度
 *   ALL:<角度>        例: ALL:45   所有舵机转到45度
 *   status            打印当前状态
 *   help              显示帮助
 */

#include <Arduino.h>
#include <Servo.h>

// ============ 配置 ============
#define NUM_SERVOS      10          // 舵机数量
#define MIN_ANGLE       0
#define MAX_ANGLE       180

// 舵机信号引脚 (D2-D11)
const int servoPins[NUM_SERVOS] = {
    2,  3,  4,  5,  6,  7,  8,  9,   // 舵机 0-7
    10, 11                            // 舵机 8-9
};

// ============ 全局变量 ============
Servo servos[NUM_SERVOS];           // 舵机对象数组
int currentAngles[NUM_SERVOS];      // 当前角度
String inputBuffer = "";            // 串口输入缓冲

// ============ 函数声明 ============
void processCommand(String cmd);
void setServoAngle(int id, int angle);
void printStatus();
void printHelp();
bool isValidServo(int id);
bool isValidAngle(int angle);

// ============ 初始化 ============
void setup() {
  Serial.begin(115200);
  delay(500);

  // 绑定所有舵机到引脚
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    currentAngles[i] = 90;
    servos[i].write(90);
  }

  Serial.println("========================================");
  Serial.println("  10-CH Servo Control Example");
  Serial.println("========================================");
  Serial.println("Commands:");
  Serial.println("  S<id>:<angle>  - Set servo (e.g., S0:90)");
  Serial.println("  ALL:<angle>    - All servos (e.g., ALL:45)");
  Serial.println("  status         - Show status");
  Serial.println("  help           - Show help");
  Serial.println("========================================");
  Serial.print("> ");
}

// ============ 主循环 ============
void loop() {
  // 处理串口输入
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\r' || c == '\n') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
        Serial.println();
        Serial.print("> ");
      }
    } else if (c >= 32 && c < 127) {
      inputBuffer += c;
      Serial.print(c);
    }
  }
}

// ============ 处理指令 ============
void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "STATUS") {
    printStatus();
    return;
  }

  if (cmd == "HELP") {
    printHelp();
    return;
  }

  // ALL:<angle>
  if (cmd.startsWith("ALL:")) {
    int angle = cmd.substring(4).toInt();
    if (isValidAngle(angle)) {
      for (int i = 0; i < NUM_SERVOS; i++) {
        setServoAngle(i, angle);
      }
      Serial.print("All servos set to ");
      Serial.println(angle);
    }
    return;
  }

  // S<id>:<angle>
  if (cmd.startsWith("S")) {
    int colonPos = cmd.indexOf(':');
    if (colonPos > 1) {
      int id = cmd.substring(1, colonPos).toInt();
      int angle = cmd.substring(colonPos + 1).toInt();
      if (isValidServo(id) && isValidAngle(angle)) {
        setServoAngle(id, angle);
        Serial.print("Servo ");
        Serial.print(id);
        Serial.print(" set to ");
        Serial.println(angle);
      }
    }
    return;
  }

  Serial.print("Unknown command: ");
  Serial.println(cmd);
}

// ============ 设置舵机角度 ============
void setServoAngle(int id, int angle) {
  currentAngles[id] = angle;
  servos[id].write(angle);
}

// ============ 打印状态 ============
void printStatus() {
  Serial.println("Servo Status:");
  Serial.println("----------------------------------------");
  for (int i = 0; i < NUM_SERVOS; i++) {
    Serial.print("  S");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(currentAngles[i]);
    Serial.println(" deg");
  }
  Serial.println("----------------------------------------");
}

// ============ 打印帮助 ============
void printHelp() {
  Serial.println("Available Commands:");
  Serial.println("  S<id>:<angle>  - Set servo to angle (e.g., S0:90)");
  Serial.println("  ALL:<angle>    - Set all servos (e.g., ALL:45)");
  Serial.println("  status         - Show all servo status");
  Serial.println("  help           - Show this help");
}

// ============ 校验函数 ============
bool isValidServo(int id) {
  if (id < 0 || id >= NUM_SERVOS) {
    Serial.print("Error: Servo ID must be 0-");
    Serial.println(NUM_SERVOS - 1);
    return false;
  }
  return true;
}

bool isValidAngle(int angle) {
  if (angle < MIN_ANGLE || angle > MAX_ANGLE) {
    Serial.print("Error: Angle must be ");
    Serial.print(MIN_ANGLE);
    Serial.print("-");
    Serial.println(MAX_ANGLE);
    return false;
  }
  return true;
}
