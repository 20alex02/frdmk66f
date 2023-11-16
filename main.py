from machine import Pin
led = Pin(25, Pin.OUT)
led.value(1)

spi = machine.SPI(0,
                  baudrate=100000,
                  polarity=0,
                  phase=0,
                  bits=8,
                  firstbit=machine.SPI.MSB,
                  sck=machine.Pin(2),
                  mosi=machine.Pin(3),
                  miso=machine.Pin(4))

def print_on_UART():
        data= bytearray(b'\x00'*17)
        #spi.write(b'\x00')
        spi.write_readinto(b'\xCC0123456789abcdef', data)
        return data[0]
    
def read_ADC():
        data= bytearray(b'\x00'*3)
        #spi.write(b'\x00')
        spi.write_readinto(b'\x55aa', data)
        int_val = int.from_bytes(data[1:], "big")
        return data

def read_count():
        data = spi.read(1)
        return int.from_bytes(data, "big")

print_on_UART()        
