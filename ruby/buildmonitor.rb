#!/usr/bin/env ruby

require 'serialport'
require 'cctray'

class BuildRadiator
  ARDUINO_SERIAL_PORT = '/dev/tty.usbmodem14631'

  STATUS_COMMAND = {
    'Building' => 'y',
    'Failure'  => 'r',
    'Success'  => 'g'
  }

  def initialize
    @serial = SerialPort.new(ARDUINO_SERIAL_PORT, 9600, 8, 1, SerialPort::NONE)
  end

  def send(command)
    @serial.write command
  end

  def notify(status)
    send STATUS_COMMAND[status]
  end

end

buildradiator = BuildRadiator.new
cctray        = CCTray.new("https://snap-ci.com/mavcunha/misc/cctray.xml")

while true do
  puts "Fetching build status"
  p = cctray.fetch.first

  status = p.activity == 'Building' ? p.activity : p.last_build_status

  puts "Notifying build radiator of status: #{status}"
  buildradiator.notify status

  sleep 10
end

