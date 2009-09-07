/* js:pkmk:start */
function choiceIsADowngrade() {
	return  checkChoiceUpgradeAction() ;
}
/* js:pkmk:end */


/* js:pkmk:start */
function checkChoiceUpgradeAction() {
	var result = false;
	try {
		result = my.choice.packageUpgradeAction == 'downgrade' || my.choice.packageUpgradeAction == 'mixed';
	} catch (e) {}
	
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
					anyOrAll = 1; 
					connector = ""; 
					msg = ""; 
					msgTitle = ""; 
					rules = (
							 {
								 argumentField = ""; 
								 argumentStringField = ""; 
								 booleanType = true; 
								 choiceProperty = packageUpgradeAction; 
								 comboType = downgrade; 
								 comparator = "=="; 
								 mainChoice = "Current Choice"; 
								 numberType = ""; 
								 pathField = ""; 
								 pathKeyKeyField = ""; 
								 pathKeyPathField = ""; 
								 specificChoice = choice0; 
								 stringType = ""; 
							 }, 
							 {
								 argumentField = ""; 
								 argumentStringField = ""; 
								 booleanType = true; 
								 choiceProperty = packageUpgradeAction; 
								 comboType = mixed; 
								 comparator = "=="; 
								 mainChoice = "Current Choice"; 
								 numberType = ""; 
								 pathField = ""; 
								 pathKeyKeyField = ""; 
								 pathKeyPathField = ""; 
								 specificChoice = choice0; 
								 stringType = ""; 
							 }
							 ); 
					title = "Check Choice Upgrade Action"; 
					type = 0; 
				}
				); 
	 functionName = choiceIsADowngrade; 
 }
 %%%Requirements Editor%%%
 */