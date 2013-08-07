<?php
function property_list($cat="all")
{

if(!$_SESSION['property'])
{
$rentquery="rent > '0'";
$furnishedquery="(furnished = 'Furnished') or (furnished = 'Unfurnished')";
$bedquery="bedrooms > '0'";
}else{
$p=$_SESSION['property'];
//print_r($_SESSION);
drupal_add_css(drupal_get_path('module', 'property') .'/property.css');
unset($_SESSION['property']);
$rent=$p['rent'];
$bed=$p['bedrooms'];
$f=$p['furnished'];

//Rent Query
if($rent=="1")
$rentquery="rent < '400'";
if($rent=="2")
$rentquery="(rent > '400') and (rent <= '700') ";
if($rent=="3")
$rentquery="(rent > '700') and (rent <= '1000') ";
if($rent=="4")
$rentquery="rent > '1000'";
if($rent=="any")
$rentquery="rent > '0'";

//Furnished Query
if($f=="any")
$furnishedquery="(furnished = 'Furnished') or (furnished = 'Unfurnished')";
if($f=="f")
$furnishedquery="furnished = 'Furnished'";
if($f=="uf")
$furnishedquery="furnished = 'Unfurnished'";

//BedRooms Query
if($bed=="1")
$bedquery="bedrooms = '1'";
if($bed=="2")
$bedquery="bedrooms = '2'";
if($bed=="3")
$bedquery="bedrooms = '3'";
if($bed=="4")
$bedquery="bedrooms = '4'";
if($bed=="5")
$bedquery="bedrooms = '5'";
if($bed=="any")
$bedquery="bedrooms > '0'";
}
if(($rentquery!='')&&($furnishedquery!='')&&($bedquery!=''))
{
if($cat!="All")
$sql='SELECT count(*) FROM {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.') and(category="'.$cat.'"))';
else
$sql='SELECT count(*) FROM {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.'))';
$sqlcount=$sql;
$_SESSION['count']=$sqlcount;
if($cat!="All")
$sql='SELECT * FROM {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.') and(category="'.$cat.'")) order by area';
else
$sql='SELECT * FROM {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.')) order by area';
$sqlquery=$sql;
$_SESSION['sqlquery']=$sqlquery;
}
$count = db_result(db_query($_SESSION['count']));
//echo 'SELECT count(*) FROM {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.'))';
//echo $count;
//echo $_SESSION['count'];
//echo $_SESSION['sqlquery'];
if($count > 0) 
{
	
	//echo 'select * from {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.'))';

	$res=pager_query($_SESSION['sqlquery'], 5, 0, NULL, $count);
	
	$op.="<table class=\"sortable\"><thead><tr><td colspan='2'></td></tr></thead><tbody>";
//print_r($res);
	while($arr=db_fetch_array($res))
	{
		//print_r($arr);
		$img=explode(',',$arr[images]);
		
		//$fid=db_result(db_query('select fid from {image} where ((nid="'.$img[0].'") and (image_size="_original"))'));
	    //$filepath=db_result(db_query('select filepath from {files} where fid="'.$fid.'"'));
		$op.="<tr><td valign='top' width='25%'>".image_display(node_load($img[0]), 'thumbnail')."<!--<img src='http://".$_SERVER['HTTP_HOST']."/testsite/".$filepath."' /></td>--><td align=\"left\" width='75%'><b>".$arr['area']."<br/>Rent: &pound;".$arr['rent']."pcm <br/><br/>Available From: ".$arr['availabledate']."<br/><br/></b>".$arr['summary']."<br/><br/>".$arr['furnished']."<br/><b>Council Tax:</b>".$arr['taxband']."<br/><br/> <a href='http://maps.google.co.uk/?q=".$arr['postcode']."' target='_blank'>Location <img src='http://".$_SERVER['HTTP_HOST']."/sites/all/modules/property/images/images.jpeg' border='0' height='30' style='text-decoration: none;' width='30'/></a>&nbsp;<a href='http://".$_SERVER['HTTP_HOST']."/property/requestappointment/".$arr['pid']."' target='_blank'>Enquiry <img src='http://".$_SERVER['HTTP_HOST']."/sites/all/modules/property/images/appointment.jpeg' border='0' height='30' width='30'/></a>&nbsp;<a href='http://".$_SERVER['HTTP_HOST']."/property/viewdetails/".$arr['pid']."' target='_blank'> More Information <img src='http://".$_SERVER['HTTP_HOST']."/sites/all/modules/property/images/infor.jpeg' border='0' height='30' width='30'/></a>&nbsp;<br/><a href='http://www.upmystreet.com/local/my-neighbours/l/".$arr['postcode'].".html' target='_blank'> More about Location <img src='http://".$_SERVER['HTTP_HOST']."/sites/all/modules/property/images/locationdiscovery.jpeg' border='0' height='30' width='30'/></a></td></tr>";
		$op.="<tr><td></td></tr><tr><td colspan='2'><hr/></td></tr>";
	 
	  
	}
	$op.="</tbody></table>";
	$op.=theme('pager', NULL, 5, 0);
}
else
{
	$op.="No Properties in this Region";
}	

  //echo $op;
return $op;
}
function property_map($cat="all")
{


 drupal_set_html_head('<script src="'. check_url(url('http://maps.googleapis.com/maps/api/js?key='.variable_get('property_google_maps_API', 'ABQIAAAANUKs-h2CoZG3fHpV9IoPgxTPbml2QEMqDSpmRaZZLxEpcvTMVBTteCmJPhRTVx-rgMA2crvSiTm2Cw').'&sensor=false')) .'" type="text/javascript"></script>');
 
 
drupal_set_html_head('<script src="'. check_url(url('http://www.google.com/uds/api?file=uds.js&v=1.0&key='.variable_get('property_google_ajax_search_API', 'ABQIAAAANUKs-h2CoZG3fHpV9IoPgxTPbml2QEMqDSpmRaZZLxEpcvTMVBTteCmJPhRTVx-rgMA2crvSiTm2Cw'))) .'" type="text/javascript"></script>');

//drupal_set_html_head('<script src="'. check_url(url('http://gmaps-utility-library.googlecode.com/svn/trunk/markermanager/release/src/markermanager_packed.js')) .'" type="text/javascript"></script>');

//$external_js = 'http://'.$_SERVER['HTTP_HOST'].'/testsite/'.drupal_get_path('module', 'property') . '/gmap.js';
drupal_add_js(drupal_get_path('module', 'property') . '/gmap.js');

//$external_js = 'http://'.$_SERVER['HTTP_HOST'].'/testsite/'.drupal_get_path('module', 'property') . '/infobubble.js';
//drupal_add_js(drupal_get_path('module', 'property') . '/infobubble.js');

if(!$_SESSION['property'])
{
$rentquery="rent > '0'";
$furnishedquery="(furnished = 'Furnished') or (furnished = 'Unfurnished')";
$bedquery="bedrooms > '0'";
}else{
$p=$_SESSION['property'];
drupal_add_css(drupal_get_path('module', 'property') .'/property.css');
unset($_SESSION['property']);
$rent=$p['rent'];
$bed=$p['bedrooms'];
$f=$p['furnished'];

//Rent Query
if($rent=="1")
$rentquery="rent < '400'";
if($rent=="2")
$rentquery="(rent > '400') and (rent <= '700') ";
if($rent=="3")
$rentquery="(rent > '700') and (rent <= '1000') ";
if($rent=="4")
$rentquery="rent > '1000'";
if($rent=="any")
$rentquery="rent > '0'";

//Furnished Query
if($f=="any")
$furnishedquery="(furnished = 'Furnished') or (furnished = 'Unfurnished')";
if($f=="f")
$furnishedquery="furnished = 'Furnished'";
if($f=="uf")
$furnishedquery="furnished = 'Unfurnished'";

//BedRooms Query
if($bed=="1")
$bedquery="bedrooms = '1'";
if($bed=="2")
$bedquery="bedrooms = '2'";
if($bed=="3")
$bedquery="bedrooms = '3'";
if($bed=="4")
$bedquery="bedrooms = '4'";
if($bed=="5")
$bedquery="bedrooms = '5'";
if($bed=="any")
$bedquery="bedrooms > '0'";
}
if(($rentquery!='')&&($furnishedquery!='')&&($bedquery!=''))
{
	if($cat!='All')
		$count = db_result(db_query('SELECT count(*) FROM {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.')and (category="'.$cat.'"))'));
	else
			$count = db_result(db_query('SELECT count(*) FROM {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.'))'));

if($count > 0) 
{
	if($cat!="All")
		$sqlquery='select * from {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.') and (category="'.$cat.'"))';
	else
		$sqlquery='select * from {property_details} where (('.$rentquery.') and ('.$furnishedquery.') and ('.$bedquery.') )';
	
	$res=db_query($sqlquery);
	$property=array();
	$i=0;
	while($arr=db_fetch_array($res))
	{
		$img=explode(',',$arr[images]);
		$postcode=$arr['postcode'];
		$tooltip="<strong>".$arr['area']."</strong><br/>".$arr['bedrooms']." Bedrooms<br/><strong>Rent:</strong>&nbsp;&pound;".$arr['rent']."&nbsp;per&nbsp;month";
		$fid=db_result(db_query('select fid from {image} where ((nid="'.$img[0].'") and (image_size="thumbnail"))'));
	    $filepath=db_result(db_query('select filepath from {files} where fid="'.$fid.'"'));
		$op.="<table ><tbody>";
		$op.="<tr><td valign='top' width='25%'><img src='http://".$_SERVER['HTTP_HOST']."/".$filepath."' width='75px' height='75px'/></td><td align='left' ><font size='1'><b>".$arr['area']."<br/>Rent: &pound;".$arr['rent']."pcm <br/>Available From: ".$arr['availabledate']."<br/></b>".$arr['summary']."<br/>".$arr['furnished']."<br/><b>Council Tax:</b>".$arr['taxband']."<br/> <a href='http://maps.google.co.uk/?q=".$arr['postcode']."' target='_blank'>Location <img src='http://".$_SERVER['HTTP_HOST']."/sites/all/modules/property/images/images.jpeg' border='0' height='30' style='text-decoration: none;' width='30'/></a>&nbsp;<br/><a href='http://".$_SERVER['HTTP_HOST']."/property/requestappointment/".$arr['pid']."' target='_blank' >Enquiry <img src='http://".$_SERVER['HTTP_HOST']."/sites/all/modules/property/images/appointment.jpeg' border='0' height='30' width='30'/></a>&nbsp;<br/><a href='http://".$_SERVER['HTTP_HOST']."/property/viewdetails/".$arr['pid']."' target='_blank'> More Information <img src='http://".$_SERVER['HTTP_HOST']."/sites/all/modules/property/images/infor.jpeg' border='0' height='30' width='30'/></a>&nbsp;<br/><br/><a href='http://www.upmystreet.com/local/my-neighbours/l/".$arr['postcode'].".html' target='_blank'> More about Location <img src='http://".$_SERVER['HTTP_HOST']."/sites/all/modules/property/images/locationdiscovery.jpeg' border='0' height='30' width='30'/></a></font></td></tr>";
		$op.="</tbody></table>";
	 $property[$i]['tooltip']=$tooltip;
	 $property[$i]['data']=$op;
	 $property[$i]['postcode']=$postcode;
 
	 $op='';
	  $i++;
	}
	
}
	$finalproperty=array();
	$postcode=array();
	for($j=0;$j<count($property);$j++)
	{
	  $postcode[$j]=$property[$j]['postcode'];
	}
	$arr=Array();
	$k=0;

	for($i=0;$i<count($postcode);$i++)
	{
		$val=true;
		for($l=0;$l<count($arr);$l++)
		{
			for($m=0;$m<count($arr[$l]);$m++)
			{
				if($arr[$l][$m]==$i)
				{
				 $val=false;
				 echo $val;
				  break 2;
				}
				 else
				 {
				 $val=true;	
				 }
			 }
			 
		}
		for($j=0;$j<count($postcode);$j++)
		{
			if(($val)&&($j>$i)&&($i!=$j)&&($postcode[$i]==$postcode[$j]))
			{
				
				$arr[$i][$k]=$i;$k++;
				$arr[$i][$k]=$j;$k++;
				
			}
		}
	sort($arr);
	}
	for($j=0;$j<count($arr);$j++)
	{
	$arr[$j]=array_unique($arr[$j]);
	sort($arr[$j]);
	}
	
	for($i=0;$i<count($property);$i++)
	{	
		if(count($arr)>0)
		{
		for($j=0;$j<count($arr);$j++)
		{
			for($k=0;$k<count($arr[$j]);$k++)	
			{
				if($arr[$j][$k]==$i)
				{
				}
				else
				{
					$finalproperty[$i]=$property[$i];
				}
			}
		}
		}
		else
		{
				$finalproperty[$i]=$property[$i];
		}
	}
	sort($finalproperty);
	$count=count($finalproperty)-1;
	for($i=0;$i<count($arr);$i++)
	{
		 $count++;
	
		for($j=0;$j<count($arr[$i]);$j++)
		{
		   
			$finalproperty[$count]['tooltip'].=$property[$arr[$i][$j]]['tooltip']."<br/>";
			$finalproperty[$count]['data'].=$property[$arr[$i][$j]]['data'];
			$finalproperty[$count]['postcode']=$property[$arr[$i][$j]]['postcode'];
	
		}
		 
	}
	

}	

$ot='<div id="map" style="width:540px; height:540px"></div>';


$ot.='<script type="text/javascript">//<![CDATA[ 
		//addLoadEvent(mapLoad('.variable_get("property_lat","54.622978").','.variable_get("property_long","-2.592773").'));
   // addLoadEvent(showMap("'.variable_get("property_lat","54.622978").'","'.variable_get("property_long","-2.592773").'"));
		addLoadEvent(function() {mapLoad('.variable_get("property_lat","54.622978").','.variable_get("property_long","-2.592773").');';


for($i=0;$i<count($finalproperty);$i++)
{	
	$postcode=$finalproperty[$i]['postcode'];
  $tooltip=$finalproperty[$i]['tooltip'];
  $data=$finalproperty[$i]['data'];
  $ot.='usePointFromPostcode_top("'.$postcode.'","'.$tooltip.'","'.$data.'",cbfunction);';

}
//55.619612","-4.476328
$ot.='	showMap("'.variable_get("property_lat","54.622978").'","'.variable_get("property_long","-2.592773").'",'.variable_get("property_zoom","13").');
			
		});

	//]]></script>';
 
return $ot;
}


function property_appointment(){
$URLexplosion = explode('/', str_replace('//', '/', $_SERVER['REQUEST_URI']));
$URLlastpart = (count($URLexplosion)-1);
$content = $URLexplosion[$URLlastpart];
$area=db_result(db_query('select area from {property_details} where pid='.$content));
$postcode=db_result(db_query('select postcode from {property_details} where pid='.$content));
$form = array(
    '#type' => 'fieldset',
    '#title' => t('Property Enquiry Request form'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
	'#submit' => array('property_appointment_submit'),
  );
  $form['#prefix'] ='Enquiry for Property '.$area.' '.$postcode;
  $form['area'] = array(
    '#type' => 'hidden',
    '#default_value' => $area,
   );
   $form['pc'] = array(
    '#type' => 'hidden',
    '#default_value' => $postcode,
   );
    $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => '',
    '#required' => TRUE,
  );
    $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('email'),
    '#default_value' => '',
    '#required' => TRUE,
  );
   $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact Number'),
    '#default_value' => '',
    '#required' => TRUE,
  );
   $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => '', 
    '#cols' => 30, 
    '#rows' => 5, 
    '#description' => t('Enter the details of your enquiry here.'),
    '#required' => TRUE,
  );

    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Send',
  );
  
    return $form;
}
function property_viewdetails()
{
	drupal_add_css(drupal_get_path('module', 'property') .'/property.css');
	$URLexplosion = explode('/', str_replace('//', '/', $_SERVER['REQUEST_URI']));
	$URLlastpart = (count($URLexplosion)-1);
	$pid = $URLexplosion[$URLlastpart];
	$before=$URLexplosion[$URLlastpart-1];
	//echo $pid;
	//echo $before;
	if($before=="viewdetails")
	{
	$count=db_result(db_query('select count(*) from {property_details} where pid='.$pid));
	//echo $count;
	if($count>0)
	{
		$res=db_query('select * from {property_details} where pid='.$pid);
		while($row=db_fetch_array($res))
		{
			$arr=array();
			$arr=$row;
			$area=$arr['area'];
			$rent=$arr['rent'];
			$bed=$arr['bedrooms'];
			$ad=$arr['availabledate'];
			$des=$arr['description'];
			$pc=$arr['postcode'];
			$lettype=$arr['lettype'];
			$reception=$arr['receptionrooms'];
			$bathrooms=$arr['bathrooms'];
			$kitchen=$arr['kitchen'];
			$heating=$arr['heating'];
			$propertytype=$arr['propertytype'];
			$floor=$arr['floor'];
			$buildingstyle=$arr['buildingstyle'];
			$availabledate=$arr['availabledate'];
			$furnished=$arr['furnished'];
			$taxband=$arr['taxband'];
			$parking=$arr['parking'];
			$postcode=$arr['postcode'];
			$restrictions=$arr['resctrictions'];
			$additional=$arr['additionalfeatures'];
			$garden=$arr['garden'];
			$img=explode(',',$arr['images']);
			
		}
	   $op='<div><div class="propertylistingone">
          <h2>'.$area.'</h2>
          <p><strong>Rent:</strong> &pound;'.$rent.' per month<br />
          <strong>Bedrooms:</strong> '.$bed.'<br />
		  <strong>Kitchen:</strong> '.$kitchen.'<br />
		  <strong>Bathroom:</strong> '.$bathrooms.'<br />
		  <strong>Reception Rooms:</strong> '.$reception.'<br />
          <strong>Availability Date:</strong>  '.$ad.'</p>
                    <p>'.$des.'</p>
					<p> <h2>Additional Details</h2>
          
            <strong>Furnished</strong>: '.$furnished.'<br/>
            <strong>Property Type</strong>: '.$propertytype.'<br/>
			<strong>Let Type</strong>: '.$lettype.'<br/>
            <strong>Floor</strong>: '.$floor.'<br/>

            <strong>Building Style</strong>: '.$buildingstyle.'<br/>
            <strong>Heating</strong>: '.$heating.'<br/>
            <strong>Parking</strong>: '.$parking.'<br/>
            <strong>Garden</strong>: '.$garden.'<br/>
            <strong>Council Tax Band</strong>: '.$taxband.'<br/>

            <strong>Restrictions</strong>: '.$restrictions.'<br/>
            <strong>Additional Features</strong>: '.$additional.'<br/>
          </p>
          <div>
<a href="http://maps.google.co.uk/?q='.$arr['postcode'].'" target="_blank">Location <img src="http://'.$_SERVER['HTTP_HOST'].'/sites/all/modules/property/images/images.jpeg" border="0" height="30" style="text-decoration: none;" width="30"/></a>&nbsp;<a href="http://'.$_SERVER['HTTP_HOST'].'/property/requestappointment/'.$arr['pid'].'" target="_blank" >Enquiry <img src="http://'.$_SERVER['HTTP_HOST'].'/sites/all/modules/property/images/appointment.jpeg" border="0" height="30" width="30"/></a>&nbsp;<br/><a href="http://www.upmystreet.com/local/my-neighbours/l/'.$arr['postcode'].'.html" target="_blank"> More about Location <img src="http://'.$_SERVER['HTTP_HOST'].'/sites/all/modules/property/images/locationdiscovery.jpeg" border="0" height="30" width="30"/></a>
</div>

 </div>';
		$filepaths=array();
		for($i=0;$i<(count($img));$i++)
		{
		$fid=db_result(db_query('select fid from {image} where ((nid="'.$img[$i].'") and (image_size="_original"))'));
	$filepath=db_result(db_query('select filepath from {files} where fid="'.$fid.'"'));
	if($filepath!='')
	$filepaths[$i]="http://".$_SERVER['HTTP_HOST']."/".$filepath;
	}
        $op.='<div class="propertylistingtwo">
          <a id="primarygallerylink" href="'.$filepaths[0].'" rel="lightshow[gallery]"><!--<img src="'.$filepaths[0].'" alt="" id="primarydetail" width="86%" align="left"/>-->'.image_display(node_load($img[0]), 'primarypropertyimg').'</a>';
for($i=1;$i<(count($filepaths));$i++)
{
$op.='<a class="othergallerylink" href="'.$filepaths[$i].'" rel="lightshow[gallery]"><!--<img src="'.$filepaths[$i].'" alt="'.$area.'" class="gallery" />-->'.image_display(node_load($img[$i]), 'propertythumbnail').'</a>';
}
$val=((count($filepaths)-1)%3);
if($val!=0)
{
//echo "in if";
for($k=1;$k<$val;$k++)
{
//echo "in for";
$op.='<img src="http://'.$_SERVER['HTTP_HOST'].'/sites/all/modules/property/images/galleryfiller.png" alt="" class="gallery" />';
}
}
$op.='<p id="gallerylegend" class="screenhelper" ><span>Click on an image to see an enlarged view</span></p>         
        </div>
		</div>
      ';
	return $op;
	}
	else
	{
		drupal_set_message('No Property with that Property Id');
	}
	}
	else
	{
		drupal_set_message('No Property with that Property Id');
	}
}
