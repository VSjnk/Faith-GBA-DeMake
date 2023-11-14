//audio playback stuff will go here

void PlaySoundDMA() {

	
	REG_SOUNDCNT_H = 0x0B0F; //0x0b0F; //enable DS A&B + FIFO reset + use timer0 + max volume to L and R
	REG_SOUNDCNT_X = 0x0080; //turn sound chip on
		
		REG_DMA1SAD = _testSound; // src

	//DMA1 destination
	REG_DMA1DAD = 0x040000a0; //write to FIFO A address 
	REG_DMA1CNT_H = 0xb600; //DMA control: DMA enabled+ start on FIFO+32bit+repeat+increment source&dest

	//REG_TM0D
	//Formula for playback frequency is: 0xFFFF-round(cpuFreq/playbackFreq) 
	REG_TM0CNT_L =  0xFBE6; //16khz playback frequency
	REG_TM0CNT_H = 0x0080; //enable timer
}