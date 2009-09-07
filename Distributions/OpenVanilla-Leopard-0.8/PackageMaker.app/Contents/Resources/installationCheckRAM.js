/* js:pkmk:start */
function installationCheckRAM() {
	return  checkMemsize() ;
}
/* js:pkmk:end */


/* js:pkmk:start */
function checkMemsize() {
	var result = false;
	try {
		result = system.sysctl('hw.memsize') >= '134217728';
	} catch (e) {}
	
	if(!result) {
		my.result.type = 'Warn';
		my.result.title = 'Memory Warning';
		my.result.message = 'You have less than 128 MB of RAM.';
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
					msg = "You have less than 128 MB of RAM."; 
					msgTitle = "Memory Warning"; 
					rules = (
							 {
								 argumentField = "hw.memsize"; 
								 argumentStringField = ""; 
								 booleanType = true; 
								 choiceProperty = enabled; 
								 comboType = ""; 
								 comparator = ">="; 
								 mainChoice = "Result of Sysctl"; 
								 numberType = ""; 
								 pathField = ""; 
								 pathKeyKeyField = ""; 
								 pathKeyPathField = ""; 
								 specificChoice = choice0; 
								 stringType = 134217728; 
							 }
							 ); 
					title = "Check memsize"; 
					type = 2; 
				}
				); 
	 functionName = installationCheckRAM; 
 }
 %%%Requirements Editor%%%
 */