var tagN = ["error", "warning", "info"];
var tagI = [false, false, false];

var xmlDoc;

function handleXML(elementName, tagName)
{
	var messages  = xmlDoc.getElementsByTagName(tagName);

	if(messages.length === 0)
	{
		return false;
	}

	var table = document.createElement("table");

	table.className = "messageTable";

	var tableBody = document.createElement("tbody");

	table.appendChild(tableBody);

	for(var i = 0; i < messages.length; i++)
	{
		var line = document.createElement("tr");

		if((i % 2) === 0)
		{
			line.className = "evenn";
		}

		var cell1 = document.createElement("td");
		var cell2 = document.createElement("td");

		cell1.className = "cell1"; 

		var message = document.createTextNode(messages[i].childNodes[0].nodeValue);

		cell1.appendChild(document.createTextNode(i+1 + "."));
		cell2.appendChild(message);

		line.appendChild(cell1);
		line.appendChild(cell2);

		tableBody.appendChild(line);
	}

	document.getElementById(elementName).appendChild(table);

	return true;
}

function openXML(url)
{
	var xmlHttp;

	try
	{
		xmlHttp = new XMLHttpRequest();
	}
	catch(e1)
	{
		try
		{
			xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
		}
		catch(e2)
		{
			alert("Ajax is not supported. - XMLHttpRequest");

			return false;
		}
	}

	try
	{
		xmlHttp.open("GET", url, false);
		xmlHttp.send();
		xmlDoc = xmlHttp.responseXML;

		return true;
	}
	catch(e3)
	{
		alert("Error is occurred by reading file with messages (" + e3 + ").");

		return false;
	}
}

function readXML(url, elementName, tagName)
{
	if(xmlDoc == undefined)
	{
		if(!openXML(url))
		{
			return;
		}
	}

	var i = 0
	for(; i < tagN.length; i++)
	{
		if(tagN[i] === tagName)
		{
			if(tagI[i] === true)
			{
				return;
			}

			break;
		}
	}

	if(handleXML(elementName, tagName))
	{
		tagI[i] = true;
	}
}

function showHide(elementName, linkName)
{
	var element = document.getElementById(elementName);
	var link = document.getElementById(linkName);

	if(element.children.length === 0)
	{
		link.innerHTML = "&#9650;";
	}
	else
	{
		if(element.style.display === "none")
		{
			element.style.display = "block";
			link.innerHTML = "&#9650;";
		}
		else
		{
			element.style.display = "none";
			link.innerHTML = "&#9660;";
		}
	}
}