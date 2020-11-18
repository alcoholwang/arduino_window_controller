#include <EEPROM.h>

#define POSITIVE_PIN 9
#define NEGATIVE_PIN 11
#define STORE_ADDRESS 0

const int postiveValue = 50;
const int stopValue = 25;
const int negativeValue = 10;

const String postiveReceiveCommand = "@POSITIVE$";
const String stopReceiveCommand = "@STOP$";
const String negativeReceiveCommand = "@NEGATIVE$";

const String queryReceiveCommand = "@???$";

const String postiveReturnCommand = "!POSITIVE$";
const String stopReturnCommand = "!STOP$";
const String negativeReturnCommand = "!NEGATIVE$";

int currentState;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(POSITIVE_PIN, OUTPUT);
  pinMode(NEGATIVE_PIN, OUTPUT);
  Serial1.begin(9600);

  reset();

  switch (currentState) {
    case postiveValue:
      positive();
      break;
    case stopValue:
      stop();
      break;
    default:
      negative();
      break;
  }
}

void reset() {
  currentState = EEPROM.read(STORE_ADDRESS);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(POSITIVE_PIN, LOW);
  digitalWrite(NEGATIVE_PIN, LOW);
}

void stop() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(POSITIVE_PIN, LOW);
  digitalWrite(NEGATIVE_PIN, LOW);
  currentState = stopValue;
  EEPROM.update(STORE_ADDRESS, currentState);
  Serial1.println(stopReturnCommand);
}

void positive() {
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(POSITIVE_PIN, HIGH);
  digitalWrite(NEGATIVE_PIN, LOW);
  currentState = postiveValue;
  EEPROM.update(STORE_ADDRESS, currentState);
  Serial1.println(postiveReturnCommand);
}

void negative() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(POSITIVE_PIN, LOW);
  digitalWrite(NEGATIVE_PIN, HIGH);
  currentState = negativeValue;
  EEPROM.update(STORE_ADDRESS, currentState);
  Serial1.println(negativeReturnCommand);
}

void loop()
{
  while (Serial1.available()) {
    String command = Serial1.readString();
    if (command.startsWith(postiveReceiveCommand)) {
      positive();
    }
    if (command.startsWith(negativeReceiveCommand)) {
      negative();
    }
    if (command.startsWith(stopReceiveCommand)) {
      stop();
    }
    if (command.startsWith(queryReceiveCommand)) {
      switch (currentState) {
        case postiveValue:
          Serial1.println(postiveReturnCommand);
          break;
        case stopValue:
          Serial1.println(stopReturnCommand);
          break;
        default:
          Serial1.println(negativeReturnCommand);
          break;
      }
    }
  }
}
