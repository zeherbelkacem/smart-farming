// Initialiser le fournisseur d'informations d'identification Amazon Cognito
AWS.config.region = 'eu-west-2'; // Région
AWS.config.credentials = new AWS.CognitoIdentityCredentials({
    IdentityPoolId: 'eu-west-2:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx', //Replace with your IdentityPoolId
});


var dynamodb = new AWS.DynamoDB();
var recentDatetime = new Date() - 86400000; //13 digits
var recentDatetime = parseInt(recentDatetime/1000); //10 digits 


var params = { TableName: 'final_project',

        KeyConditionExpression: '#id = :iddd and #dt >= :lastdt', 
        ExpressionAttributeNames:{
            "#id": "DeviceId",
	    "#dt": "PosixTime"
        },
        ExpressionAttributeValues: {
            ":iddd":{'N' : '12'},
	    ":lastdt": { "N" : recentDatetime.toString()}
        }
};
/*****weather table******/
var params1 = {TableName: 'weathermap'};

$(function() {
  getTemperature();
  $.ajaxSetup({ cache: false });
  setInterval(getTemperature, 200000);
});

var colors = ["#1D507A", "#2F6999", "#66A0D1", "#8FC0E9", "#4682B4"];

function getTemperature() {
	var temperature_data = []; 
	var humidity_data = []; 
	var first_date = 0;
	dynamodb.scan(params1, function(err, data) {
		if (err) {
			console.log(err);
			return null;
		} else {
			for (var i in data['Items']) {
			if(i < 15){//w'll display just the first (recent) fifteen data
				temperature_data[i] = {"x" : parseFloat(data["Items"][i]['datetime']['N']), "y": parseFloat(data["Items"][i]['temperature']['N'])};
				humidity_data[i] = {"x" : parseFloat(data["Items"][i]['datetime']['N']), "y": parseFloat(data["Items"][i]['humidity']['N'])};
				}
			}
		var temp_data = [{
			key: 'Temperature',
           	values: temperature_data
        }]
		var humid_data = [{
			key: 'Humidity',
			"color":"green",
           	values: humidity_data
        }]
		console.log(temp_data);
		console.log(humid_data);
		//first_date = data['Items'][0]['datetime']['N'];
		}

		nv.addGraph(function() {
			var chartTemp =nv.models.multiBarChart() // Initialise the lineChart object.
			.useInteractiveGuideline(true); // Turn on interactive guideline (tooltips) 
			//chart.brushExtent([new Date(first_date), new Date(first_date + 24*3600*1000)]); // 24*3600*1000ms = 1jour
			chartTemp.xAxis
				
				.showMaxMin(false)
				.tickFormat(function(d) {
					return d3.time.format('%H:00 (%a)')(new Date(d))
					}); // Set the label of the xAxis (Vertical)
			chartTemp.yAxis //Chart y-axis settings
				.showMaxMin(false)
				.axisLabel('Temperature (°c)')
				.tickFormat(d3.format('.00f'));
				 // Rounded Numbers Format.
			d3.select('#Temperature svg') // Select the ID of the html element we defined earlier.
				.datum(temp_data) // Pass in the JSON
				.transition().duration(500) // Set transition speed
				.call(chartTemp); // Call & Render the chart
			nv.utils.windowResize(chartTemp.update); // Intitiate listener for window resize so the chart responds and changes width.
			
			var chartHumid = nv.models.multiBarChart() // Initialise the lineChart object.
			
			.useInteractiveGuideline(true); // Turn on interactive guideline (tooltips) 
			//chart.brushExtent([new Date(first_date), new Date(first_date + 24*3600*1000)]); // 24*3600*1000ms = 1jour
			chartHumid.xAxis
				.showMaxMin(false)
				.tickFormat(function(d) {
					return d3.time.format('%H:00 (%a)')(new Date(d))
					}); // Set the label of the xAxis (Vertical)
			chartHumid.yAxis //Chart y-axis settings
				.showMaxMin(false)
				.axisLabel('Humidity (°c)')
				.tickFormat(d3.format('.00f'));
				 // Rounded Numbers Format.
			d3.select('#Humidity svg') // Select the ID of the html element we defined earlier.
				.datum(humid_data) // Pass in the JSON
				.transition().duration(500) // Set transition speed
				.call(chartHumid); // Call & Render the chart
			nv.utils.windowResize(chartHumid.update); // Intitiate listener for window resize so the chart responds and changes width.
		return; 
		});
  	});
	
 
}

 
/*initial values for powerLevel and waterLevel*/
var powerLevel = 5;
var waterLevel = 5;
/*periodic jQuerry method get the device power level  */
$(document).ready(function() {
    var progression = 0,
    progress1 = setInterval(function() 
    {
        $('#progress .progress-text').text(progression + '%');
        $('#progress .progress-bar').css({'width':progression + '%'});
        if(progression == powerLevel) {
            clearInterval(progress);

        } else
            progression += 5;

    }, 1000);
});
/* jQuerry method get the device water level  */
$(document).ready(function(){
	$(".myButtonClass2").click(function(){
		/* aws api gateway endpoint url to trigger aws lamda allowing button click to send action (irrigate the plant)  to the device */
		$.get("https://dis0codu38.execute-api.eu-west-2.amazonaws.com/irrigate/irrigate", function(data, status){
		  alert("Data: " + data + "\nStatus: " + status);
		});
	 });
});
/* jQuerry method get the device water level  */
$(document).ready(function(){
	$(".myButtonClass1").click(function(){
		/*aws api gateway endpoint url to trigger aws lamda allowing button click to send action (get sensor data value)  from the device */
		$.get("https://dis0codu38.execute-api.eu-west-2.amazonaws.com/getdata/getdata", function(data, status){
		  alert("Data: " + data + "\nStatus: " + status);
		});
	 });
});
/*periodic jQuerry method get the device water level  */
$(document).ready(function() {
    var progression = 0,
    progress = setInterval(function() 
    {
        $('#progress1 .progress-text1').text(progression + '%');
        $('#progress1 .progress-bar1').css({'width':progression + '%'});
        if(progression == waterLevel) {
            clearInterval(progress);

        } else
            progression += 5;

    }, 1000);
});
// Create the context for applying the chart to the HTML canvas 
var ctx = $("#graph").get(0).getContext("2d");

// Set the options for our chart 
var options = { segmentShowStroke : false,
					animateScale: true,
					percentageInnerCutout : 20,
					showToolTips: true,
					tooltipEvents: ["mousemove", "touchstart", "touchmove"],
					tooltipFontColor: "#fff",
					animationEasing : 'easeOutCirc'
					}
// Set the inital data 
var init = [
	{
		 value: 1,
		color: "blue",
		highlight: "#2980b9",
		label: "Fruit color"
  }
			];
graph = new Chart(ctx).Doughnut(init, options);
/*update function for  the query temp , soil humidyt and fruit color graph*/
$(function() {
  getData();
  $.ajaxSetup({ cache: false });
  setInterval(getData, 30000);
});
var fullColorHex = function(r,g,b) {   
  var red = rgbToHex(r);
  var green = rgbToHex(g);
  var blue = rgbToHex(b);
  return red+green+blue;
};
var rgbToHex = function (rgb) { 
  var hex = Number(rgb).toString(16);
  if (hex.length < 2) {
       hex = "0" + hex;
	
  }
  return hex;
};
/* Makes a Query of the DynamoDB table (final_project) to set a data object for the chart 
-Query the rgb data ans choose the last  value to update the color graph
-Query the water level data and choose the last value and update the water level progress bar (update waterLevel variable)
-Query the power level data and chose the last value and update the power progress bar graph (update powerLevel variable)
*/
function getData() {
  dynamodb.query(params, function(err, data) {
    if (err) {
      console.log(err);
      return null;
    } else {
      var redColor = 0;
      var greenColor = 0;
      var blueColor = 0;
      var batteryLevel = " ";
      var waterLevell = " ";

		for (var i in data['Items']) {
		redColor = parseFloat(data['Items'][i]['payload']['M']['Data']['M']['RGB']['L'][0]['N']);
		greenColor = parseFloat(data['Items'][i]['payload']['M']['Data']['M']['RGB']['L'][1]['N']);
		blueColor = parseFloat(data['Items'][i]['payload']['M']['Data']['M']['RGB']['L'][2]['N']);
		batteryLevel = data['Items'][i]['payload']['M']['Data']['M']['Pw']['S'];
		waterLevell = data['Items'][i]['payload']['M']['Data']['M']['WL']['S'];
		}

		if(batteryLevel == "full"){
		powerLevel = 85;}
		else if(batteryLevel == "medium"){
		powerLevel = 50;} 
		if(batteryLevel == "low"){
		powerLevel = 35;}
		if(waterLevell == "empty"){
		waterLeve = 20;}
		else if(waterLevell == "medium"){
		waterLeve = 50;}
		else if(waterLevell == "full"){
		waterLeve = 85;}
		var newColor = "#" + fullColorHex(redColor, greenColor, blueColor);
	   
		 var dat = [ { value: 1, color:"black", highlight: "#c0392b", label: "Fruit Color" },];
		dat[0].color = newColor;

      // Only update if we have new values (preserves tooltips)
		  if (  graph.segments[0].fillColor != dat[0].color)
		  {
			graph.segments[0].fillColor = dat[0].color;
			graph.segments[0].highlightColor = dat[0].color;

			graph.update();
		  }

    }
  });
}

