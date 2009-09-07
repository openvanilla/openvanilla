/* js:pkmk:start */
function volumeCheckTiger() {
	return  checkSystemversion() ;
}
/* js:pkmk:end */


/* js:pkmk:start */
function checkSystemversion() {
	var result = false;
	try {
		result = my.target.systemVersion.ProductVersion >= '10.4';
	} catch (e) {}
	
	if(!result) {
		my.result.type = 'Fatal';
		my.result.title = '';
		my.result.message = '';
	}
	return result;
}
/* js:pkmk:end */


/* ******* DO NOT EDIT ANYTHING BELOW THIS LINE ******* */
/* THE TEXT BELOW IS THE DEFINITION FOR THE REQUIREMENTS EDITOR */

/*
 %%%Requirements Editor%%%
 {
	 clauses = (
				{
					anyOrAll = 2; 
					connector = ""; 
					msg = ""; 
					msgTitle = ""; 
					rules = (
							 {
								 argumentField = ""; 
								 argumentStringField = ""; 
								 booleanType = true; 
								 choiceProperty = enabled; 
								 comboType = ""; 
								 comparator = ">="; 
								 mainChoice = "Target OS Version (e.g. 10.x.x)"; 
								 numberType = ""; 
								 pathField = ""; 
								 pathKeyKeyField = ""; 
								 pathKeyPathField = ""; 
								 specificChoice = choice0; 
								 stringType = "10.4"; 
							 }
							 ); 
					title = "Check systemVersion"; 
					type = 3; 
				}
				); 
	 functionName = volumeCheckTiger; 
 }
 %%%Requirements Editor%%%
 */