#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Ultrasonic sensor pins
const int trigPin = 2;  // D4
const int echoPin = 0;  // D3

// Variables for ultrasonic sensor
long duration;
int distance;

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  
  // Initialize I2C communication for OLED display
  Wire.begin();
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  
  // Initialize I2C communication for MLX90614 sensor
  mlx.begin();

  // Setup pins for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Measure distance using ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance
  distance = duration * 0.034 / 2;

  // Check if the person is detected within 1 meter range
  if (distance <= 100) { // 100 cm = 1 meter
    // Read temperature from MLX90614 sensor
    float tempC = mlx.readObjectTempC();
    
    // Display temperature on OLED display
    display.clearDisplay();
    display.setTextSize(1);      
    display.setTextColor(SSD1306_WHITE); 
    display.setCursor(0,0);     
    display.print("Temperature: ");
    display.print(tempC);
    display.println(" *C");
    display.display();
  } else {
    // Person not detected within 1 meter range
    display.clearDisplay();
    display.setTextSize(1);      
    display.setTextColor(SSD1306_WHITE); 
    display.setCursor(0,0);     
    display.println("Not Detected");
    display.display();
  }

  // Delay before next reading
  delay(2000);
}
