var map;
var localSearch = new GlocalSearch();

var icon = new GIcon();
icon.image = "http://www.google.com/mapfiles/marker.png";
icon.shadow = "http://www.google.com/mapfiles/shadow50.png";
icon.iconSize = new GSize(20, 34);
icon.shadowSize = new GSize(37, 34);
icon.iconAnchor = new GPoint(10, 34);

var bounds, redIcon, blueIcon, greenIcon, points, marker, zoom;
points = [];

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

    
function usePointFromPostcode(postcode,callbackFunction) {
var localSearch1 = new GlocalSearch();
	localSearch1.setSearchCompleteCallback(null,
		function() {

			if (localSearch1.results[0]) {
				var resultLat = localSearch1.results[0].lat;
				var resultLng = localSearch1.results[0].lng;
				var point = new GLatLng(resultLat,resultLng);
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
//alert("maploaded");
var localSearch1 = new GlocalSearch();
	localSearch1.setSearchCompleteCallback(null,
		function() {

			if (localSearch1.results[0]) {
				var resultLat = localSearch1.results[0].lat;
				var resultLng = localSearch1.results[0].lng;
				var point = new GLatLng(resultLat,resultLng);
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

function mapLoad(l,ll) {
	if (GBrowserIsCompatible()) {
		map = new GMap2(document.getElementById("map"));
		map.addControl(new GSmallMapControl());
		map.addControl(new GMapTypeControl());
	//	map.setCenter(new GLatLng(54.622978,-2.592773), 5, G_NORMAL_MAP);
  //alert(l);
  //alert(ll);
    map.setCenter(new GLatLng(l,ll), 5, G_NORMAL_MAP);
		bounds = new GLatLngBounds();
	}
}

function createMarker(point, icon, title, html) {
	var marker = new GMarker(point, { icon:icon });
	var tooltip = new Tooltip(marker, title, 4);
		
	marker.tooltip = tooltip;
	map.addOverlay(marker);
	map.addOverlay(tooltip);
	
	GEvent.addListener(marker, "click", function() {
		this.tooltip.hide();
		map.openInfoWindowHtml(point, html);
	});

	GEvent.addListener(marker,'mouseover', function(){
		this.tooltip.show();
	});
	GEvent.addListener(marker,'mouseout', function(){
		this.tooltip.hide();
	});

	return marker;
}

function createMarker(point, icon, title, html) {
	var marker = new GMarker(point, { icon:icon });
	var tooltip = new Tooltip(marker, title, 4);
		
	marker.tooltip = tooltip;
	map.addOverlay(marker);
	map.addOverlay(tooltip);
	
	if(html!='') {
	GEvent.addListener(marker, "click", function() {
		this.tooltip.hide();
		map.openInfoWindowHtml(point, html);
	});
	}

		GEvent.addListener(marker,'mouseover', function(){
			this.tooltip.show();
		});
		GEvent.addListener(marker,'mouseout', function(){
			this.tooltip.hide();
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
	
	var point = new GLatLng(lat, lon);
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
	map.setCenter(new GLatLng(l,ll));
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
addUnLoadEvent(GUnload);