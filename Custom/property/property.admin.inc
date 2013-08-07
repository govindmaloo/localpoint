<?php
function properties_admin() {
  $form = array();

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('RECIPIENTS'),
    '#default_value' => variable_get('properties_email', 'lakella@ces.co.uk'),
    '#size' => 50,
    '#maxlength' => 50,
    '#description' => t("Email Address for Appointments.Example: 'webmaster@example.com' or 'sales@example.com,support@example.com'. To specify multiple recipients, separate each e-mail address with a comma."),
    '#required' => TRUE,
  );
  $form['GoogleMapsAPI'] = array(
    '#type' => 'textfield',
    '#title' => t('GoogleMapsAPI'),
    '#default_value' => variable_get('property_google_maps_API', 'ABQIAAAANUKs-h2CoZG3fHpV9IoPgxTPbml2QEMqDSpmRaZZLxEpcvTMVBTteCmJPhRTVx-rgMA2crvSiTm2Cw'),
    '#size' => 100,
    '#maxlength' => 150,
    '#description' => t("Google Maps API Key...Unique for each Domain"),
    '#required' => TRUE,
  );
   $form['GoogleAjaxSearchAPI'] = array(
    '#type' => 'textfield',
    '#title' => t('GoogleAjaxSearchAPI'),
    '#default_value' => variable_get('property_google_ajax_search_API', 'ABQIAAAANUKs-h2CoZG3fHpV9IoPgxTPbml2QEMqDSpmRaZZLxEpcvTMVBTteCmJPhRTVx-rgMA2crvSiTm2Cw'),
    '#size' => 100,
    '#maxlength' => 150,
    '#description' => t("Google Ajax Search API Key...Unique for each Domain"),
    '#required' => TRUE,
  );
   $form['centerlat'] = array(
    '#type' => 'textfield',
    '#title' => t('Center of the map Lat'),
    '#default_value' => variable_get('property_lat', '54.622978'),
    '#size' => 100,
    '#maxlength' => 150,
    '#description' => t("GCenter of the map Lat"),
    '#required' => TRUE,
  );
   $form['centerlong'] = array(
    '#type' => 'textfield',
    '#title' => t('Center of the map Long'),
    '#default_value' => variable_get('property_long', '-2.592773'),
    '#size' => 100,
    '#maxlength' => 150,
    '#description' => t("GCenter of the map Long"),
    '#required' => TRUE,
  );
  $form['zoom'] = array(
    '#type' => 'textfield',
    '#title' => t('zoom of the map'),
    '#default_value' => variable_get('property_zoom', '13'),
    '#size' => 100,
    '#maxlength' => 150,
    '#description' => t("Map zoom"),
    '#required' => TRUE,
  );
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
	  '#submit' => array('properties_admin_submit'),
      '#weight' => 9,
    );

  return $form;
}
function property_admin_categories_list_show() {
   $result = db_query('SELECT category,cid FROM {property_category} ');
  
  $count = db_result(db_query("SELECT COUNT(*) FROM {property_category}"));

 // echo $count."count";
  //$destination = drupal_get_destination();
  $output = '<div style="width:350px">';
   if ($count>0) { 
  while ($category = db_fetch_object($result)) {
   
      $output.= '<div id="leftcol" style="float:left">'.$category->category.'</div><div nowrap id="rightcol" style="float:right"><a href="http://'.$_SERVER['HTTP_HOST'].'/admin/content/property/category/'. $category->cid .'/edit" > EDIT </a></div><br/>';
    
  }

  
  } else {
    $output.='No Property categories have been created.';
  }
 $output.="</div>";
  return $output;
}
function property_category_edit(&$form_state, $type, $cid = NULL) {

	if ($type == 'add') {
  $form['#prefix'] = 'Please complete all of the required fields and click \'Submit\' to insert your property category.';

	// Category title
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Category title'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	 $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      //'#access' => !in_array($menu['menu_name'], menu_list_system_menus()),
      '#submit' => array('property_category_insert_form_submit'),
      '#weight' => 9,
    );
	} else {
  //check that an entry exists

  $check = db_query('SELECT count(*) FROM {property_category} WHERE cid ='.$cid.'');
  $count = db_result($check);
   
  if($count >0) {
	//get repair details using CID
	$result = db_query('SELECT category,cid FROM {property_category} WHERE cid ='.$cid.' LIMIT 1');
	$category = db_fetch_object($result);
	$form['#prefix'] = 'Make any amendments below and click \'Save\' to update your changes. Or click \'Delete\' to delete this property category.';


	// Category ID
    $form['cid'] = array(
      '#type' => 'hidden',
      '#default_value' => $cid,
    );
	 $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Category title'),
      '#default_value' => $category->category,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	$form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      //'#access' => !in_array($menu['menu_name'], menu_list_system_menus()),
      '#submit' => array('property_category_edit_form_submit'),
      '#weight' => 9,
    );
	// Delete function - only available on editing
	$form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      //'#access' => ,
      '#validate' => array('property_category_delete_form_submit'),
      '#weight' => 10,
    );
	} else {
     //Form introduction text
    $form['text'] = array(
    '#type' => 'markup',
    '#value' => '<p>There are no details for the category selected.</p>'
    );
    }
	}
    return $form;
 }
function property_add($form_values=NULL)
{
 $form['#prefix'] = 'Please complete all of the required fields and click \'Submit\' to insert a new property.';
	$result = db_query('SELECT category FROM {property_category}');
	$options=array();
	$options[1]="no category";
	while ($category = db_fetch_object($result)) {
      $options[$category->category] = $category->category; 
    }
	$form['category'] = array(
      '#type' => 'select',
      '#title' => t('Select the region'),
      '#default_value' => '',
        '#required' => TRUE,
	  '#options' => $options,
    );
    $form['area'] = array(
      '#type' => 'textfield',
      '#title' => t('AreaName'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	 $form['summary'] = array(
      '#type' => 'textfield',
      '#title' => t('Summary of Property'),
      '#default_value' => '',
      '#maxlength' => 255,
      '#description' => t('Enter the Summary of property.'),
      '#required' => TRUE,
    );
    $form['desc'] = array(
      '#type' => 'textarea',
      '#title' => t('Description of Property'),
      '#default_value' => '',
      '#cols' => 30,
      '#rows' => 5,
      '#description' => t('Enter the description of the property.'),
      '#required' => TRUE,
    );
 
    $form['rent'] = array(
      '#type' => 'textfield',
      '#title' => t('Rent PCM'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	$form['bedrooms'] = array(
      '#type' => 'select',
      '#title' => t('No of Bedrooms'),
      '#default_value' => '',
        '#required' => TRUE,
	  '#options' => array (t('0'), t('1'), t('2'), t('3'), t('4'), t('5'), t('6'), t('7'), t('8'), t('9'), t('10')),
    );
	$form['receptionrooms'] = array(
      '#type' => 'select',
      '#title' => t('No. of Reception Rooms'),
      '#default_value' => '',
       '#required' => FALSE,
	  '#options' => array (t('0'), t('1'), t('2'), t('3'), t('4'), t('5'), t('6'), t('7'), t('8'), t('9'), t('10')),
    );
	$form['bathrooms'] = array(
      '#type' => 'select',
      '#title' => t('No. of Bathrooms'),
      '#default_value' => '',
      '#required' => FALSE,
	  '#options' => array (t('0'), t('1'), t('2'), t('3'), t('4'), t('5')),
    );
	 $form['kitchen'] = array(
      '#type' => 'textfield',
      '#title' => t('Kitchen'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	 $form['heating'] = array(
      '#type' => 'textfield',
      '#title' => t('Heating'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	 $form['propertytype'] = array(
      '#type' => 'textfield',
      '#title' => t('Property Type'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	$form['floor'] = array(
      '#type' => 'textfield',
      '#title' => t('Floor'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['buildingstyle'] = array(
      '#type' => 'textfield',
      '#title' => t('Building Style'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['availabledate'] = array(
      '#type' => 'textfield',
      '#title' => t('Available Date'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	$form['lettype'] = array(
      '#type' => 'textfield',
      '#title' => t('Let Type'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 255,
      '#required' => FALSE,
    );
	$form['furnished'] = array(
      '#type' => 'select',
      '#title' => t('Furnished'),
      '#default_value' => '',
      '#required' => TRUE,
	  '#options' => array ('Furnished'=>t('Furnished'),'Unfurnished'=> t('Unfurnished')),
    );
	$form['taxband'] = array(
      '#type' => 'textfield',
      '#title' => t('CouncilTax Band'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['parking'] = array(
      '#type' => 'textfield',
      '#title' => t('Parking'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['garden'] = array(
      '#type' => 'textfield',
      '#title' => t('Garden'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['postcode'] = array(
      '#type' => 'textfield',
      '#title' => t('Postcode'),
      '#default_value' => '',
      '#size' => 60,
      '#maxlength' => 8,
	  '#description' => t('Please enter the space also when entering the postcode.'),
      '#required' => TRUE,
    );
	 $form['restrictions'] = array(
      '#type' => 'textarea',
      '#title' => t('Restrictions'),
      '#default_value' => '',
      '#cols' => 30,
      '#rows' => 5,
      '#description' => t('Enter the restrictions of the property.'),
      '#required' => FALSE,
    );
	$form['additionalfeatures'] = array(
      '#type' => 'textarea',
      '#title' => t('Additional Features'),
      '#default_value' => '',
      '#cols' => 30,
      '#rows' => 5,
      '#description' => t('Enter the additional features of the property'),
      '#required' => FALSE,
    );
	$form['Images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload the images according to the order you want to appear. First image will be primary image'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  for($i=0;$i<15;$i++)
  {
	$form['Images']['image'.$i] = array(
	'#type' => 'file',
	'#title' => t('Images of the  property'),
	'#size' => 60,
	'#description' => t('Upload your image.'),
	'#maxlength' => 250,
	//'#required' => TRUE,
	);
}
	
	
    $form['#insert'] = TRUE;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      //'#access' => !in_array($menu['menu_name'], menu_list_system_menus()),
      '#submit' => array('property_insert_form_submit'),
      '#weight' => 9,
    );

 $form['#attributes']['enctype'] = 'multipart/form-data';
 return $form;
}

function properties_select_page()
{
 drupal_add_css(drupal_get_path('module', 'property') .'/property.css');
  $result = db_query('SELECT pid, area FROM {property_details} ');
  
  $count = db_result(db_query("SELECT COUNT(*) FROM {property_details}"));

 // echo $count."count";
  //$destination = drupal_get_destination();
  $output = '<div style="width:350px">';
   if ($count>0) { 
  while ($category = db_fetch_object($result)) {
   
      $output.= '<div style="float:left">'.$category->area.'</div><div nowrap style="float:right"><a href="http://'.$_SERVER['HTTP_HOST'].'/admin/content/property/'. $category->pid .'/edit" > EDIT </a></div><br/>';
    
  }

  
  } else {
    $output.='No repair categories have been created.';
  }
 $output.="</div>";
  return $output;
}
function property_details_edit(&$form_state, $pid = NULL)
{
	//check that an entry exists
  $check = db_query('SELECT pid FROM {property_details} WHERE pid ='.$pid.'');
  $count = db_result($check);
  if($count >0) {
	//get property details using PID
	$result = db_query('SELECT * FROM {property_details} WHERE pid ='.$pid.' LIMIT 1');
	$category = db_fetch_object($result);
	$form['#prefix'] = 'Make any amendments below and click \'Save\' to update your changes. Or click \'Delete\' to delete this Property.';
	 $form['pid'] = array(
      '#type' => 'hidden',
      '#default_value' => $pid,
     );
	$result = db_query('SELECT category FROM {property_category}');
	$options=array();
	$options[1]="no category";
	while ($category1 = db_fetch_object($result)) {
      $options[$category1->category] = $category1->category; 
    }
	$form['category'] = array(
      '#type' => 'select',
      '#title' => t('Select the region'),
      '#default_value' =>$category->category,
        '#required' => TRUE,
	  '#options' => $options,
    );
    $form['area'] = array(
      '#type' => 'textfield',
      '#title' => t('AreaName'),
      '#default_value' => $category->area,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
 
    $form['desc'] = array(
      '#type' => 'textarea',
      '#title' => t('Description of Property'),
      '#default_value' => $category->description,
	  '#cols' => 30,
      '#rows' => 5,
      '#description' => t('Enter the description of the property.'),
      '#required' => TRUE,
    );
	 $form['summary'] = array(
      '#type' => 'textfield',
      '#title' => t('Summary of Property'),
      '#default_value' => $category->summary,
      '#maxlength' => 255,
      '#description' => t('Enter the summary of the property.'),
      '#required' => TRUE,
    );
 
    $form['rent'] = array(
      '#type' => 'textfield',
      '#title' => t('Rent PCM'),
      '#default_value' => $category->rent,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	$form['bedrooms'] = array(
      '#type' => 'select',
      '#title' => t('No of Bedrooms'),
      '#default_value' => $category->bedrooms,
        '#required' => TRUE,
	  '#options' => array (t('0'), t('1'), t('2'), t('3'), t('4'), t('5'), t('6'), t('7'), t('8'), t('9'), t('10')),
    );
	$form['receptionrooms'] = array(
      '#type' => 'select',
      '#title' => t('No. of Reception Rooms'),
      '#default_value' => $category->receptionrooms,
       '#required' => FALSE,
	  '#options' => array (t('0'), t('1'), t('2'), t('3'), t('4'), t('5'), t('6'), t('7'), t('8'), t('9'), t('10')),
    );
	$form['bathrooms'] = array(
      '#type' => 'select',
      '#title' => t('No. of Bathrooms'),
      '#default_value' => $category->bathrooms,
      '#required' => FALSE,
	  '#options' => array (t('0'), t('1'), t('2'), t('3'), t('4'), t('5')),
    );
	 $form['kitchen'] = array(
      '#type' => 'textfield',
      '#title' => t('Kitchen'),
      '#default_value' => $category->kitchen,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	 $form['heating'] = array(
      '#type' => 'textfield',
      '#title' => t('Heating'),
      '#default_value' => $category->heating,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	 $form['propertytype'] = array(
      '#type' => 'textfield',
      '#title' => t('Property Type'),
      '#default_value' => $category->propertytype,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	$form['floor'] = array(
      '#type' => 'textfield',
      '#title' => t('Floor'),
      '#default_value' => $category->floor,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['buildingstyle'] = array(
      '#type' => 'textfield',
      '#title' => t('Building Style'),
      '#default_value' =>$category->buildingstyle,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['availabledate'] = array(
      '#type' => 'textfield',
      '#title' => t('Available Date'),
      '#default_value' => $category->availabledate,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => TRUE,
    );
	$form['lettype'] = array(
      '#type' => 'textfield',
      '#title' => t('Let Type'),
      '#default_value' => $category->lettype,
      '#size' => 60,
      '#maxlength' => 255,
      '#required' => FALSE,
    );
	$form['furnished'] = array(
      '#type' => 'select',
      '#title' => t('Furnished'),
      '#default_value' => $category->furnished,
      '#required' => TRUE,
	  '#options' => array ('Furnished'=>t('Furnished'),'Unfurnished'=> t('Unfurnished')),
    );
	$form['taxband'] = array(
      '#type' => 'textfield',
      '#title' => t('CouncilTax Band'),
      '#default_value' => $category->taxband,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['parking'] = array(
      '#type' => 'textfield',
      '#title' => t('Parking'),
      '#default_value' => $category->parking,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['garden'] = array(
      '#type' => 'textfield',
      '#title' => t('Garden'),
      '#default_value' =>$category->garden,
      '#size' => 60,
      '#maxlength' => 100,
      '#required' => FALSE,
    );
	$form['postcode'] = array(
      '#type' => 'textfield',
      '#title' => t('Postcode'),
      '#default_value' =>$category->postcode,
      '#size' => 60,
      '#maxlength' => 8,
	  '#description' => t('Please enter the space also when entering the postcode.'),
      '#required' => TRUE,
    );
	 $form['restrictions'] = array(
      '#type' => 'textarea',
      '#title' => t('Restrictions'),
      '#default_value' => $category->resctrictions,
      '#cols' => 30,
      '#rows' => 5,
      '#description' => t('Enter the description of the repair category.'),
      '#required' => FALSE,
    );
	$form['additionalfeatures'] = array(
      '#type' => 'textarea',
      '#title' => t('Additional Features'),
      '#default_value' =>$category->additionalfeatures,
      '#cols' => 30,
      '#rows' => 5,
      '#description' => t('Enter the description of the repair category.'),
      '#required' => FALSE,
    );
	$form['Images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload the images according to the order you want to appear. First image will be primary image'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

$image=explode(',',$category->images);
$count=sizeof($image);
for($i=0;$i<$count;$i++)
{
   $fid=db_result(db_query('select fid from {image} where ((nid="'.$image[$i].'") and (image_size="thumbnail"))'));
	$filepath=db_result(db_query('select filepath from {files} where fid="'.$fid.'"'));
	$form['Images']['text'.$i] = array(
  '#type' => 'markup',
  '#value' => '<div class=\"form-item\" id=\"edit-existing-image-wrapper\"><label for=\"edit-existing-image\"><strong>Existing Property Image:</strong></label><br />'.image_display(node_load($image[$i]), 'thumbnail').'</div>'
	);
	if($image[$i]!="")
	{
		$form['Images']['img'.$i] =array (
		'#type' => 'checkbox',
		'#title' => t('Delete Property Image '),
		);
	}
	$form['Images']['image'.$i] = array(
	'#type' => 'file',
	'#title' => t('New Image of the property'),
	'#size' => 60,
	'#description' => t('Upload your new image if you wish to change the existing image.'),
	'#maxlength' => 250,
	//'#required' => TRUE,
	);

}
if($count!=15)
{
  for($i=($count-1);$i<15;$i++)
  {
	$form['Images']['image'.$i] = array(
	'#type' => 'file',
	'#title' => t('Images of the  property'),
	'#size' => 60,
	'#description' => t('Upload your image.'),
	'#maxlength' => 250,
	//'#required' => TRUE,
	);
 }
}
	$form['#Update'] = TRUE;
	$form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      //'#access' => !in_array($menu['menu_name'], menu_list_system_menus()),
      '#submit' => array('property_edit_form_submit'),
      '#weight' => 9,
    );
	// Delete function - only available on editing
	$form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      //'#access' => ,
      '#validate' => array('property_delete_form_submit_validate'),
      '#weight' => 10,
    );
    } else {
     //Form introduction text
    $form['text'] = array(
    '#type' => 'markup',
    '#value' => '<p>There are no properties for the selected.</p>'
    );
    }
	
  $form['#attributes']['enctype'] = 'multipart/form-data';
  return $form;
}
