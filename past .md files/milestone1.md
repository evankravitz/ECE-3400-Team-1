<meta http-equiv="X-UA-Compatible" content="IE=Edge,chrome=1">
# Milestone 1

The goal of this milestone was to implement line following and program our robot to drive in a figure eight.

## Hardware Modifications

We began by attaching the light sensors to the bottom of the robot. We used the line sensors provided, and began with three sensors. After seeing what other groups approached the problem with, we were uncertain of the viability of the three sensor design. However, having two sensors on either side of the line worked very well, because any time one went over the tape, we could immediately tell the robot was off course. In fact, we could likely remove the middle sensor and retain all functionality.

![](./resources/sensorposition.png)

We had some issues with getting consistent readings from the line sensors, so our first solution was to put the line sensors closer to the ground. We did this by switching out the wheels. This made our line readings significantly more consistent. The value of the middle sensor (reading the black tape) was typically over 900, while the outer sensors read between 100 and 300 on the white board.

We ran into significant hardware problems as we continued working on our project. When the robot was programmed to go straight, it would continuously veer to the left. This behavior suggests an improper servo calibration, but setting both servos to 90 resulted in no movement (so the servos must have been properly calibrated). We puzzled over this problem for several hours, switching out several servos and adjusting our code to unevenly power one servo more than the other. Probing the PWM signals at the Arduino output pins revealed that they were behaving as expected. Re-doing the wiring ended up fixing the problem. Likely, one signal connection was loose and sent an odd PWM signal to the servo.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=Cvb9fMoiSzk
" target="_blank"><img src="http://img.youtube.com/vi/Cvb9fMoiSzk/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

## Software Implementation

Our line detection algorithm was fairly simple. We observed that the sensors measured over 800 units on black tape and under 300 units (typically under 150) on the white board. We noted that if the robot moved off the tape in either direction, one of the outer sensors would be over black tape. We then could adjust wheel speeds and turn back onto the correct path. We could detect junctions when all three sensors were over the black tape.

Because we primarily tested later in the day, where light was lower, we set the black/white threshold at 400 units - this high margin ensured that we would not get any false positives - i.e., the robot would not shift unexpectedly. 

Our line detection mechanism proved to be very robust. Whenever it was tested, it could accurately detect robot position relative to the line. 

The primary challenge we faced was implementing the actual motion correction. We initially adjusted the speed of each servo by 1 to 2 units. We could not figure out why line correction was not working. The serial output told us that the robot was detecting position correctly, and that it was in fact changing the PWM signal. We hypothesized that it was in fact a servo issue - and while there was a servo hardware problem (see above) - this was not the primary issue. After several hours of testing, we finally pushed the correction servo power up significantly. One servo would move fowards at +45 (relative to 90) and the other would move at -45 (relative to 90). This gave us immediate and real results. We successfully implemented line correction!

```
  if(line[2]>400){
    rightSpeed = 45;
    Serial.println("LEFT");
    leftSpeed = 180 ;
  }
  else if(line[0]>400){
    leftSpeed =135;
    rightSpeed = 0;
    Serial.println("RIGHT");
  }
  else if (line[0]<400 && line[2]<400){
    Serial.println("STRAIGHT");
    leftSpeed = 135;
    rightSpeed = 45;
  }
```

To get the robot to turn at a junction, we specified the conditions under which the robot should continue to turn. We set the servos' speed to turn once the sensors detect a junction. Most of our code directs the robot based on whether the sensors detect that the robot is at a junction, on the line, to the left of line, or to the right of the line, but turning must be address differently. For example, as the robot turns left, the sensors (which are all currently located at the front of the bot) will first report that the robot is on the line, then left of the line, then for a period of time it will not be able to detect the line at all, then right of the line, and finally back on the line. 

Thus, we implemented code that broke from the primary loop that directs the robot based on position in reference to the line, allowing the robot to continue to turn after it turns away from the junction. Since the robot begins and ends a turn directly on the line, one of two boolean requirements must be satisified in order for the robot to continue to turn: it must either have not passed the line it originally began on, or the robot isn't straight on the line yet. This is roughly implemented as shown below: 

```
  boolean straight = (line[0]<400 && line[1]>400 && line[2]<400);
  boolean passed = false; 
  
  if (leftWheel == leftTurn | leftWheel == rightTurn){
    if {!straight | !passed) {
      if (line[1]<400) { 
        passed = true; 
      }
      break; 
    }
    passed = false; 
  }
```

Here is the video of our robot doing a figure eight:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=9BDlo4FiO3k
" target="_blank"><img src="http://img.youtube.com/vi/9BDlo4FiO3k/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>
