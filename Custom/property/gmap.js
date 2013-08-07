var map;
var localSearch = new GlocalSearch();

var blueIcon = ["http://www.google.com/mapfiles/marker.png","http://www.google.com/mapfiles/shadow50.png"];

var bounds, redIcon, blueIcon, greenIcon, points, marker, zoom;
points = [];
/*
//Change these colours
redIcon = new GIcon(G_DEFAULT_ICON);
redIcon.image = "http://www.google.com/intl/en_us/mapfiles/ms/micons/red.png";
redIcon.iconSize = new GSize(32, 32);
redIcon.shadowSize = new GSize(32, 32);
redIcon.iconAnchor = new GPoint(0,32);

blueIcon = new GIcon(G_DEFAULT_ICON);
blueIcon.image = "http://www.google.com/intl/en_us/mapfiles/ms/micons/blue.png";
blueIcon.iconSize = new GSize(32, 32);
blueIcon.shadowSize = new GSize(32, 32);
blueIcon.iconAnchor = new GPoint(0,32);

greenIcon = new GIcon(G_DEFAULT_ICON);
greenIcon.image = "http://www.google.com/intl/en_us/mapfiles/ms/micons/green.png";
greenIcon.iconSize = new GSize(32, 32);
greenIcon.shadowSize = new GSize(32, 32);
greenIcon.iconAnchor = new GPoint(0,32);
*/
    
function usePointFromPostcode(postcode,callbackFunction) {
var localSearch1 = new GlocalSearch();
	localSearch1.setSearchCompleteCallback(null,
		function() {

			if (localSearch1.results[0]) {
				var resultLat = localSearch1.results[0].lat;
				var resultLng = localSearch1.results[0].lng;
				var point = new google.maps.LatLng(resultLat,resultLng);
				callbackFunction(point);
			} else {
				alert("Postcode not found!");
			}
		});

	localSearch1.execute(postcode + ", UK");
}
function usePointFromPostcode_top(postcode,tooltip,data,callbackFunction) {
var tp=tooltip;
var dt=data;
alert("maploaded");
var localSearch1 = new GlocalSearch();
	localSearch1.setSearchCompleteCallback(null,
		function() {

			if (localSearch1.results[0]) {
				var resultLat = localSearch1.results[0].lat;
				var resultLng = localSearch1.results[0].lng;
				var point = new google.maps.LatLng(resultLat,resultLng);
				callbackFunction(tp,dt,point);
			} else {
				alert("Postcode not found!");
			}
		});

	localSearch1.execute(postcode + ", UK");
}
function cbfunction(tooltip,data,point)
{
	mapAddPoint(blueIcon, point.lat(),point.lng(),tooltip,data);
}

function placeMarkerAtPoint(point) {
	var marker = new GMarker(point,icon);
	map.addOverlay(marker);
}

function setCenterToPoint(point) {
	map.setCenter(point, 17);
}

function placeMarkerAtPointAndCentre(point) {
	var marker = new GMarker(point,icon);
	map.addOverlay(marker);
	map.setCenter(point, 15);
}

function showPointLatLng(point) {
	alert("Latitude: " + point.lat() + "\nLongitude: " + point.lng());
}

function mapLoad(lt,ll) {
	var myOptions = {
    center: new google.maps.LatLng(lt, ll),
    zoom: 5,
    mapTypeId: google.maps.MapTypeId.ROADMAP,

    // Add controls
    mapTypeControl: true,
    scaleControl: true,
    overviewMapControl: true,
    overviewMapControlOptions: {
      opened: true
    }
};

map = new google.maps.Map(document.getElementById('map'),  myOptions);	
bounds = new google.maps.LatLngBounds();
}

function createMarker(point, icon, title, html) {
	
	var marker = new google.maps.Marker({
		position: point,
		icon:icon[0],
		shadow:icon[1],
		draggable: false,
		map: map,
		title: title		
	});
	
	//var tooltip = new Tooltip(marker, title, 4);		
	//marker.tooltip = tooltip;
	//map.setMap(marker);
	//map.setMap(tooltip);
	/*infoBubble2 = new InfoBubble({
          map: map,
          content: title,
          position: point,
          shadowStyle: 1,
          padding: 0,
          backgroundColor: 'rgb(57,57,57)',
          borderRadius: 4,
          arrowSize: 10,
          borderWidth: 1,
          borderColor: '#2c2c2c',
          disableAutoPan: true,
          hideCloseButton: true,
          arrowPosition: 30,
          backgroundClassName: 'phoney',
          arrowStyle: 2
        });
		*/
	if(html!='') {
	var infowindow = new google.maps.InfoWindow({
      content: html
	});
	google.maps.event.addListener(marker, 'click', function(event) {
		//infoBubble2.close();
		infowindow.open(map,marker);
	});
	}

		google.maps.event.addListener(marker,'mouseover', function(){
			//infoBubble2.open();
		});
		google.maps.event.addListener(marker,'mouseout', function(){
			//infoBubble2.close();
		});

	return marker;
}

/* Can't use markerManager here, will reload every marker after zoom, can't be stopped by zoomend handler for some reason ... */
function loadMarkers() {
	for (var i=0,q=points.length; i<q; i++) {
		map.addOverlay(points[i]);
	}
}

function mapAddPoint(icon, lat, lon, title, html) {
	
	var point = new google.maps.LatLng(lat, lon);
	if(lat>55.85) {
		bounds.extend(point);
	}
	if(html=='') {
	  marker = createMarker(point, icon, title, '');		
	} else {
	  marker = createMarker(point, icon, title, '<div class="infowindow" style="overflow:auto; height: 250px !important; height: 250px; min-height: 250px; margin-right: 10px; width: 350px !important;">' + html + '</div>');
	}
	points.push(marker);
}

function showMap(l,ll,z) {
//	zoom = Math.min(12, map.getBoundsZoomLevel(bounds));
	map.setZoom(z);
	map.setCenter(bounds.getCenter());
	map.setCenter(new google.maps.LatLng(l,ll));
}

function addLoadEvent(func) {
	var oldonload = window.onload;
	if (typeof window.onload != 'function') {
		window.onload = func;
	} else {
		window.onload = function() {
			oldonload();
			func();
		}
	}
}

function addUnLoadEvent(func) {
	var oldonunload = window.onunload;
	if (typeof window.onunload != 'function') {
		window.onunload = func;
	} else {
		window.onunload = function() {
			oldonunload();
			func();
		}
	}
}

//addLoadEvent(mapLoad);
 //google.maps.event.addDomListener(window, 'load', mapLoad);