
#include <nn/atk.h>
#include <nns/atk/atk_SampleCommon.h>

#include "main.h"
#include "sound.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>
#include <dsound.h>

enum {
	SND_CH_BGM = 0,
	SND_CH_SE,
};

#define SE_CH_NUM	(5)

IDirectSound8* pDS8;
IDirectSoundBuffer8 *pDSB_bgm, *pDSB_se[SE_CH_NUM];
int se_ch_idx;
int vol_delay_cnt = -1;
float vol_delay_val;

namespace
{
	//const char ArchiveRelativePath[] = "SoundData.bfsar";

	//const int SoundHeapSize = 4 * 1024 * 1024;

	//nn::atk::SoundHeap          g_SoundHeap;
	//nn::atk::FsSoundArchive     g_SoundArchive;
	//nn::atk::SoundArchivePlayer g_SoundArchivePlayer;
	//nn::atk::SoundDataManager   g_SoundDataManager;

	//nn::audio::MemoryPoolType   g_MemoryPool;

	//void* g_pMemoryForSoundSystem;
	//void* g_pMemoryForSoundHeap;
	//void* g_pMemoryForInfoBlock;
	//void* g_pMemoryForSoundDataManager;
	//void* g_pMemoryForSoundArchivePlayer;
	//void* g_pMemoryForStreamBuffer;

	//nn::atk::SoundHandle        g_SoundHandleBGM;
	//nn::atk::SoundHandle        g_SoundHandleSE;
}



void InitSound()
{

	//	// SoundSystem初期化
	   //{
	   //	nns::atk::InitializeHeap();
	   //	nns::atk::InitializeFileSystem();


	   //	bool isSuccess = true;

	   //	nn::atk::SoundSystem::SoundSystemParam param;
	   //	std::size_t memSizeForSoundSystem = nn::atk::SoundSystem::GetRequiredMemSize(param);
	   //	g_pMemoryForSoundSystem = nns::atk::Allocate(memSizeForSoundSystem, nn::atk::SoundSystem::WorkMemoryAlignSize);
	   //	isSuccess = nn::atk::SoundSystem::Initialize(
	   //		param,
	   //		reinterpret_cast<uintptr_t>(g_pMemoryForSoundSystem),
	   //		memSizeForSoundSystem);
	   //	NN_ABORT_UNLESS(isSuccess, "cannot initialize SoundSystem");

	   //	// SoundHeap の初期化
	   //	g_pMemoryForSoundHeap = nns::atk::Allocate(SoundHeapSize);
	   //	isSuccess = g_SoundHeap.Create(g_pMemoryForSoundHeap, SoundHeapSize);
	   //	NN_ABORT_UNLESS(isSuccess, "cannot create SoundHeap");

	   //	// SoundArchive の初期化
	   //	const char* archiveAbsolutePath = nns::atk::GetAbsolutePath(ArchiveRelativePath);
	   //	isSuccess = g_SoundArchive.Open(archiveAbsolutePath);
	   //	NN_ABORT_UNLESS(isSuccess, "cannot open SoundArchive(%s)\n", archiveAbsolutePath);

	   //	// SoundArchive のパラメータ情報をメモリにロード
	   //	std::size_t infoBlockSize = g_SoundArchive.GetHeaderSize();
	   //	g_pMemoryForInfoBlock = nns::atk::Allocate(infoBlockSize, nn::atk::FsSoundArchive::BufferAlignSize);
	   //	isSuccess = g_SoundArchive.LoadHeader(g_pMemoryForInfoBlock, infoBlockSize);
	   //	NN_ABORT_UNLESS(isSuccess, "cannot load InfoBlock");

	   //	// SoundDataManager の初期化
	   //	std::size_t memSizeForSoundDataManager = g_SoundDataManager.GetRequiredMemSize(&g_SoundArchive);
	   //	g_pMemoryForSoundDataManager = nns::atk::Allocate(memSizeForSoundDataManager, nn::atk::SoundDataManager::BufferAlignSize);
	   //	isSuccess = g_SoundDataManager.Initialize(
	   //		&g_SoundArchive,
	   //		g_pMemoryForSoundDataManager,
	   //		memSizeForSoundDataManager);
	   //	NN_ABORT_UNLESS(isSuccess, "cannot initialize SoundDataManager");

	   //	// SoundArchivePlayer で用いるストリームバッファの初期化
	   //	// ストリームバッファはメモリプール管理されているヒープから確保する必要があります。
	   //	std::size_t memSizeForStreamBuffer = g_SoundArchivePlayer.GetRequiredStreamBufferSize(&g_SoundArchive);
	   //	g_pMemoryForStreamBuffer = nns::atk::AllocateForMemoryPool(memSizeForStreamBuffer);

	   //	// 専用のヒープをメモリプールにアタッチ
	   //	nn::atk::SoundSystem::AttachMemoryPool(&g_MemoryPool, nns::atk::GetPoolHeapAddress(), nns::atk::GetPoolHeapSize());

	   //	// SoundArchivePlayer の初期化
	   //	std::size_t memSizeForSoundArchivePlayer = g_SoundArchivePlayer.GetRequiredMemSize(&g_SoundArchive);
	   //	g_pMemoryForSoundArchivePlayer = nns::atk::Allocate(memSizeForSoundArchivePlayer, nn::atk::SoundArchivePlayer::BufferAlignSize);
	   //	isSuccess = g_SoundArchivePlayer.Initialize(
	   //		&g_SoundArchive,
	   //		&g_SoundDataManager,
	   //		g_pMemoryForSoundArchivePlayer, memSizeForSoundArchivePlayer,
	   //		g_pMemoryForStreamBuffer, memSizeForStreamBuffer);
	   //	NN_ABORT_UNLESS(isSuccess, "cannot initialize SoundArchivePlayer");
	   //}



	   //// SoundData読み込み
	   //{
	   //	bool isSuccess = true;

	   //	//↓ここに追加
	   //	isSuccess = g_SoundDataManager.LoadData(WSD_DECISION24, &g_SoundHeap);
	   //	NN_ABORT_UNLESS(isSuccess, "LoadData failed.");

	   //	isSuccess = g_SoundDataManager.LoadData(WSD_STONE_BREAK1, &g_SoundHeap);
	   //	NN_ABORT_UNLESS(isSuccess, "LoadData failed.");

	   //	isSuccess = g_SoundDataManager.LoadData(WSD_WATER_DROP1, &g_SoundHeap);
	   //	NN_ABORT_UNLESS(isSuccess, "LoadData failed.");

	   //	isSuccess = g_SoundDataManager.LoadData(WSD_WATER_DROP2, &g_SoundHeap);
	   //	NN_ABORT_UNLESS(isSuccess, "LoadData failed.");
	   //}

	UninitSound();

	// COMの初期化
	CoInitialize(NULL);

	HRESULT hr = DirectSoundCreate8(NULL, &pDS8, NULL);
	if (FAILED(hr)) return;

	se_ch_idx = 0;
}



void UninitSound()
{
	//   g_SoundArchivePlayer.Finalize();

	//   // 専用のヒープをメモリプールからデタッチ
	//   nn::atk::SoundSystem::DetachMemoryPool(&g_MemoryPool);

	//   g_SoundDataManager.Finalize();
	//   g_SoundArchive.Close();
	//   g_SoundHeap.Destroy();
	//   nn::atk::SoundSystem::Finalize();

	//   nns::atk::FreeForMemoryPool(g_pMemoryForStreamBuffer);
	//   nns::atk::Free(g_pMemoryForSoundArchivePlayer);
	//   nns::atk::Free(g_pMemoryForSoundDataManager);
	//   nns::atk::Free(g_pMemoryForInfoBlock);
	//   nns::atk::Free(g_pMemoryForSoundHeap);
	//   nns::atk::Free(g_pMemoryForSoundSystem);


	   //nns::atk::FinalizeFileSystem();
	   //nns::atk::FinalizeHeap();

	if (pDSB_bgm)
	{
		pDSB_bgm->Release();
		pDSB_bgm = NULL;
	}

	for (int i = 0; i < SE_CH_NUM; i++)
	{
		if (pDSB_se[i])
		{
			pDSB_se[i]->Release();
			pDSB_se[i] = NULL;
		}
	}

	if (pDS8)
	{
		pDS8->Release();
		pDS8 = NULL;
	}

	// COMの終了
	CoUninitialize();
}


void UpdateSound()
{
	//g_SoundArchivePlayer.Update();

	if (vol_delay_cnt >= 0)
	{
		if (!vol_delay_cnt--)
		{
			if (pDSB_bgm)
			{
				long vol = vol_delay_val == 0 ?
					DSBVOLUME_MIN : (long)(1000.0f * log10(vol_delay_val * 100 / 100.0f));

				if (vol < DSBVOLUME_MIN) vol = DSBVOLUME_MIN;
				if (vol > DSBVOLUME_MAX) vol = DSBVOLUME_MAX;

				pDSB_bgm->SetVolume(vol);
			}
		}
	}
}

void PlaySnd(int ch, const char* filename)
{
	//デバイスが動いていないとき
	if (!pDS8) return;

	HRESULT hr;
	WAVEFORMATEX wfex;
	char fn[MAX_PATH];
	strcpy(fn, "asset/");
	strcat(fn, filename);
	strcat(fn, ".wav");

	DSBUFFERDESC DSBufferDesc = {};
	IDirectSoundBuffer* ptmpBuf = 0;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Waveファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(fn, &mmioInfo, MMIO_READ);
	if (!hMmio) return; // ファイルオープン失敗

	// RIFFチャンク検索
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return;
	}

	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return;
	}

	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&wfex, fmsize);
	if (size != fmsize) {
		mmioClose(hMmio, 0);
		return;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return;
	}

	char* pData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);
	if (size != dataChunk.cksize) {
		delete[] pData;
		return;
	}

	mmioClose(hMmio, 0);

	IDirectSoundBuffer8** ppDSB;
	if (ch == SND_CH_BGM)
	{
		ppDSB = &pDSB_bgm;
	}
	else
	{
		ppDSB = &pDSB_se[se_ch_idx];
		se_ch_idx = (se_ch_idx + 1) % SE_CH_NUM;
	}
	if (*ppDSB)
	{
		(*ppDSB)->Release();
	}
	DSBufferDesc.dwSize = sizeof(DSBufferDesc);
	DSBufferDesc.dwBufferBytes = dataChunk.cksize;
	DSBufferDesc.lpwfxFormat = &wfex;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;
	DSBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	pDS8->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)ppDSB);
	ptmpBuf->Release();

	// セカンダリバッファにWaveデータ書き込み
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == (*ppDSB)->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
		memcpy(lpvWrite, pData, dwLength);
		(*ppDSB)->Unlock(lpvWrite, dwLength, NULL, 0);
		delete[] pData;
	}

	pDS8->SetCooperativeLevel(GetForegroundWindow(), DSSCL_NORMAL);
	hr = (*ppDSB)->Play(0, 0, ch == SND_CH_BGM);
}

void PlayBGM(nn::atk::SoundArchive::ItemId soundId)
{
	//g_SoundArchivePlayer.StartSound(&g_SoundHandleBGM, soundId);

	PlaySnd(SND_CH_BGM, (const char*)soundId);
}

void StopBGM()
{
	//g_SoundHandleBGM.Stop(0);

	if (pDSB_bgm)
	{
		pDSB_bgm->Stop();
	}
}

void SetVolumeBGM(float volume, int delayFrame)
{
	//g_SoundHandleBGM.SetVolume(volume, delayFrame);

	vol_delay_cnt = delayFrame;
	vol_delay_val = volume;
}

void PlaySE(nn::atk::SoundArchive::ItemId soundId)
{
	//g_SoundArchivePlayer.StartSound(&g_SoundHandleSE, soundId);

	PlaySnd(SND_CH_SE, (const char*)soundId);
}
