function deviceOfTypeExists(deviceClass, deviceName)
{
	var classObjects = system.ioregistry.matchingClass('IO' + deviceClass + 'Device');
	
	if(classObjects) {
		for( var i = 0; i < classObjects.length; i++){
			var obj = classObjects[i];
			
			if(obj[deviceClass + ' Product Name'] == deviceName)
				return true;
		}
	}
	
	return false;
}