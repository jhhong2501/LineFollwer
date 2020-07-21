# Line Follwer

## Goal

* PWM Control
* Normalize Raw Sensor Data
* Weight
* Sum of Sensor Data 
* (Not Completed) Stop
* (Not Completed) Acceleration

## Project Information

### Software

[Atmel Studio 7](http://studio.download.atmel.com/7.0.2389/as-installer-7.0.2389-full.exe "Atmel Studio 7")

[Terminal v1.9b](https://sites.google.com/site/terminalbpp/t/Terminal20130116.zip?attredirects=0&d=1 "Terminal v1.9b")

[SerialChart](http://starlino.com/data/serialchart/SerialChart_v034.zip "SerialChart")

<hr/>

### Theory

#### PID Control
<pre><code>proportional = position - 2000;
derivative = proportional - last_proportional;
integral += proportional;
last_proportional = proportional;
power_error = proportional * Kp + integral * Ki + derivative * Kd;</code></pre>
#### Normalize
<code>y = (x - min) / (max - min) * resolution</code>
#### Weight
<code>WeightData = NormallizaedData * Weight</code>
#### Sensor Control 
<code>Sum += WeightedData</code>

<hr/>

### Hardware

[Atmega128 datasheet](http://ww1.microchip.com/downloads/en/devicedoc/doc2467.pdf "Atmega128 datasheet")

[LMD18200 datasheet](https://www.ti.com/lit/ds/symlink/lmd18200.pdf?ts=1595228398915&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLMD18200 "Motor Driver")

[SI3311-H datasheet](https://www.alldatasheet.co.kr/datasheet-pdf/pdf_kor/154905/AUK/SI3311-H.html "SI3311-H")

[ST3811 datasheet](https://www.alldatasheet.com/datasheet-pdf/pdf/77990/AUK/ST3811.html "ST3811")



### Reference

[Normalizing](http://theultimatelinefollower.blogspot.com/2015/12/reading-calibrating-and-normalizing.html "Normalizing")
[PID Control](https://en.wikipedia.org/wiki/PID_controller "PID")

