//=====================
//HTML code for webpage
//=====================


const char webpageCont[] PROGMEM =
  R"=====(
<!DOCTYPE HTML>
<html>
<title>Pico W AsyncSocketServer</title>

<!---------------------------CSS-------------------------->

<style>

    h1     {font-size: 40px; color: green; text-align: center}
    h2     {font-size: 30px; color: black; text-align: center}
    h3     {font-size: 30px; color: black; text-align: center}
    
    canvas
    {
    margin-right: auto;
    margin-left: auto;
    display: block;
    }

</style>

<!--------------------------HTML-------------------------->

<body>

    <h1>Pico W AsyncSocketServerB</h1>
    <h2>ADC: <span style="color:blue" id="POTvalue">0</span> </h2> 
    <h3>Count: <span style="color:blue" id="DATAcount">0</span> </h3>
    <canvas id="canvas" width="480" height="256" style="background-color: black;"></canvas>
    
</body>

<!----------------------JavaScript------------------------>


<script>


var canvas = document.getElementById("canvas");
            var ctx = canvas.getContext("2d");
            ctx.fillStyle = "#dbbd7a";
            ctx.fill()
            
            ctx.lineWidth = "2";
            ctx.strokeStyle = 'yellow';

  var werta = 128;
  var wertb = 128;
  var n = 1;
  var anzahl = 1;

//InitWebSocket()
//function InitWebSocket(){
var websoc = new WebSocket('ws://'+window.location.hostname+':80/ws');

websoc.onopen = function() 
{
 //window.alert("Connected"); 
};


websoc.onmessage = function(evt) 
{
//document.getElementById('POTvalue').innerHTML = evt.data;
       JSONobj = JSON.parse(evt.data);
       document.getElementById('POTvalue').innerHTML = JSONobj.POT;
       var pot = parseInt(JSONobj.POT);
      
       wertb = pot; 
       n += 1;
       if (n >= canvas.width) { n = 1;}
       
       ctx.beginPath();
       ctx.moveTo(n - 1, werta);
       ctx.lineTo(n, wertb);
       ctx.stroke();

       ctx.clearRect(n+1, 0, 10, canvas.height);
       werta = wertb; 

       document.getElementById('DATAcount').innerHTML = anzahl; 
       anzahl += 1;
 };


//websoc.onclose = function() {};
           
//};

</script>
</html>
)=====";
