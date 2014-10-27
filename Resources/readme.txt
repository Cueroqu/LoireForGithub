hp不是飞船的属性，而是具体的时候再设置（用户的飞船，游戏中的飞船分别设置）

for all the angle values, unclockwise is positive if no additional comments, and the unit is degree

<airship type="1" name="PLANE" image="airship_1.png" movespeed="80" rotatespeed="30" initialangle="90" slotnumber="2" >
    <slots angleoffset="90" linearoffset="10" />
    <slots angleoffset="180" linearoffset="20" />
</airship>
type is a unique, positive value.
NAME IS THE AIRSHIP'S NAME IN CODES. ALL UPPER LETTERS.
unit of movespeed is pixel/second, rotate's is degree/second.
initialangle stands for the angle of airship in image, taking positive x direction as 0 degree.
slotnumber is an argument for convenient implementation in codes
for slots:
angleoffset is the offset to initialangle
linearoffset is the offset to the center of the airship

