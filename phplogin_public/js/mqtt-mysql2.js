#!/usr/bin/env node

var mqtt = require('mqtt'); //https://www.npmjs.com/package/mqtt
var Topic = '#'; //subscribe to all topics
var Broker_URL = 'mqtt://localhost';
var Database_URL = 'localhost';

var options = {
	clientId: 'MyMQTT',
	port: 1883,
	username: 'username',
	password: 'password',	
	keepalive : 60
};

var client  = mqtt.connect(Broker_URL, options);
client.on('connect', mqtt_connect);
client.on('reconnect', mqtt_reconnect);
client.on('error', mqtt_error);
client.on('message', mqtt_messsageReceived);
client.on('close', mqtt_close);

function mqtt_connect() {
    console.log("Connecting MQTT");
    client.subscribe(Topic, mqtt_subscribe);
};

function mqtt_subscribe(err, granted) {
    console.log("Subscribed to " + Topic);
	console.log("new line");
    if (err) {console.log(err);}
};

function mqtt_reconnect(err) {
    console.log("Reconnect MQTT");
    if (err) {console.log(err);}
	client  = mqtt.connect(Broker_URL, options);
};

function mqtt_error(err) {
    console.log("Error!");
	if (err) {console.log(err);}
};

function after_publish() {
	//do nothing
};

//receive a message from MQTT broker
function mqtt_messsageReceived(topic, message, packet) {
	console.log('Topic=' +  topic + '  Message=' + message);
	var message_str = message.toString(); //convert byte array to string
	message_str = message_str.replace(/\n$/, ''); //remove new line
	console.log('message_str=' +  message_str);
	//payload syntax: clientID,topic,message
	if (countInstances(message_str) != 1) {
		console.log("Invalid payload");
		} else {
		console.log('trying sql insert');
		insert_message(topic, message_str);
		//console.log(message_arr);
	}
};

function mqtt_close() {
	//console.log("Close MQTT");
};

////////////////////////////////////////////////////
///////////////////// MYSQL ////////////////////////
////////////////////////////////////////////////////


const mysql = require('mysql'); // or use import if you use TS
const util = require('util'); // Needed to get the await functionality for asyncronous functions such as sql query
const conn = mysql.createConnection({
  host: "localhost",
  user: "username",
  password: "password",
  database: "databasename"
});


// node native promisify
const query = util.promisify(conn.query).bind(conn);


async function insert_message(topic, message_str) {
	try {
		console.log("trap_ID = " + topic);
		var message_arr = extract_string(message_str); //split a string into an array [0] defines if fish got out or came in; 0 = got out, 1 = came in
		var action= message_arr[0];
		//console.log(message_arr[0]);
		console.log(action);
		var lengthact = message_arr[1];
		var requestlengthlow = lengthact - 5;
		var requestlengthhigh = lengthact + 5;
		//console.log(message_arr[1]);
		console.log(lengthact);
		if (action == 1){ //fish to be added with lenght = lengthact
			let rows = await query("SELECT * FROM "+ topic +"  WHERE length = '"+lengthact+"'");
			console.log(rows);
			console.log("Adding fish with length "+lengthact);
			await query("INSERT INTO " + topic +  " (length) VALUES ('" + lengthact + "')");
			console.log("1 record inserted");
			rows = await query("SELECT * FROM "+ topic +"  WHERE length = '"+lengthact+"'");
			console.log(rows);
		} else if (action == 0){ //fish to be removed with lenght = lenghtact, if no exact match found nearest size is removed
			let rows = await query("SELECT * FROM "+ topic +"  WHERE length = '"+lengthact+"'");
			console.log(rows);
			if (rows.length > 0) {
				console.log("Results found");
				console.log("deleting 1 from top");
				await query("DELETE FROM "+ topic +" WHERE length = '"+lengthact+"' ORDER BY id ASC LIMIT 1");
			} else if (rows.length < 1){
				console.log("Results not found, widening criteria");
				console.log("deleting 1 from top");
				await query("DELETE FROM "+ topic +" WHERE length BETWEEN '"+requestlengthlow+"' AND '"+requestlengthhigh+"' ORDER BY id ASC LIMIT 1");
			}
		}
	} catch ( err ) {
		throw err;
		// handle the error
	} finally {
		console.log("End of script");
	}
};

//split a string into an array of substrings
function extract_string(message_str) {
	var message_arr = message_str.split(","); //convert to array	
	return message_arr;
};	

//count number of delimiters in a string
var delimiter = ",";
function countInstances(message_str) {
	var substrings = message_str.split(delimiter);
	return substrings.length - 1;
};

