int char_in_array(char val, char * arr) {
	int i;
	for(i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
		if(arr[i] == val) {
			return 1;
		}
	}
	return 0;
}
