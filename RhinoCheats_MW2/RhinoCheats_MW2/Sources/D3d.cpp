#include "stdafx.h"

//_ReturnAddress()
#include <intrin.h> 

//========================================================================

enum DvarType_t : char {
	DVAR_TYPE_BOOL,
	DVAR_TYPE_FLOAT,
	DVAR_TYPE_FLOAT_2,
	DVAR_TYPE_FLOAT_3,
	DVAR_TYPE_FLOAT_4,
	DVAR_TYPE_INT,
	DVAR_TYPE_ENUM,
	DVAR_TYPE_STRING,
	DVAR_TYPE_FLOAT_3_COLOR,
	DVAR_TYPE_COUNT,
};

enum DvarFlags_t : short {
	DVAR_ARCHIVE = 0x1,
	DVAR_LATCH = 0x2,
	DVAR_CHEAT = 0x4,
	DVAR_USER_CREATED = 0x100,
	DVAR_SAVED = 0x200,
	DVAR_SERVERINFO = 0x400,
	DVAR_INIT = 0x800,
	DVAR_CHANGEABLE_RESET = 0x1000,
	DVAR_ROM = 0x2000,
	DVAR_EXTERNAL = 0x4000,
	//DVAR_AUTOEXEC = 0x8000
};

union DvarValue {
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	float value;
	float vec2[2];
	float vec3[3];
	float vec4[4];
	const char* string;
	char color[4];	
};

struct DvarLimitsEnumeration {
	int stringCount;
	const char** strings;

	int findIndexForString(const char* input) 
	{
		if (input == nullptr) {
			return 0;
		}

		for (auto i = 0; i < stringCount; i++) {
			if (std::strcmp(strings[i], input) == 0) {
				return i;
			}
		}

		return 0;
	}
};

struct DvarLimitsInteger {
	int min;
	int max;	
};

struct DvarLimitsValue {
	float min;
	float max;	
};

union DvarLimits {
	DvarLimitsEnumeration enumeration;
	DvarLimitsInteger integer;
	DvarLimitsValue value;
	DvarLimitsValue vector;	
};

struct dvar_s {
	const char* name;
	const char* description;
	DvarType_t type;
	DvarFlags_t flags;
	bool modified;
	DvarValue current;
	DvarValue latched;
	DvarValue reset;
	DvarLimits domain;
	dvar_s* next;
	int hashNext;	
};

enum UPNP_NatType {
	NAT_UNKNOWN = 0x0,
	NAT_OPEN = 0x1,
	NAT_MODERATE = 0x2,
	NAT_STRICT = 0x3,
};

dvar_s* FindDvar(const char* dvarName)
{
	return Call<dvar_s*>(0x5BCF30, dvarName);
}

dvar_s* m_connectTimeOut;
dvar_s* m_bandwithTestEnabled;
dvar_s* m_autoTeams;
dvar_s* m_maxTeamDifference;
dvar_s* m_requireOpenNat;
bool* m_doWeSuckAsHost;
UPNP_NatType* m_natType;

void InitHostVars()
{
	m_connectTimeOut = FindDvar("party_connectTimeout"); 
	XASSERT(m_connectTimeOut)

	m_bandwithTestEnabled = FindDvar("bandwidthtest_enable");
	XASSERT(m_bandwithTestEnabled);
	
	m_autoTeams = FindDvar("party_autoTeams");
	XASSERT(m_autoTeams);

	m_maxTeamDifference = FindDvar("party_maxTeamDiff");
	XASSERT(m_maxTeamDifference);

	m_requireOpenNat = FindDvar("requireOpenNat");
	XASSERT(m_requireOpenNat);

	m_doWeSuckAsHost = reinterpret_cast<bool*>(0x6427E66);
	m_natType = reinterpret_cast<UPNP_NatType*>(0x85CCC0);	
}


//========================================================================

namespace D3D
{	   	
	cSprite* pD3DSprite = nullptr;

	cSprite::cSprite()
	{
		pTex = nullptr;
		lpSprite = nullptr;
		pPos.x = 0;
		pPos.y = 0;
		pPos.z = 0;
		color = D3DCOLOR_RGBA(255, 255, 255, 255);
		result = false;
	}
	cSprite::cSprite(float x, float y)
	{
		pTex = nullptr;
		lpSprite = nullptr;
		pPos.x = x;
		pPos.y = y;
		pPos.z = 0;
		color = D3DCOLOR_RGBA(255, 255, 255, 255);
		result = false;
	}
	cSprite::~cSprite()
	{
		if (lpSprite)
		{
			lpSprite->Release();
			lpSprite = nullptr;
		}
		if (pTex)
		{
			pTex->Release();
			pTex = nullptr;
		}
	}
	bool cSprite::IsInit()
	{
		return result;
	}
	bool cSprite::Init(IDirect3DDevice9* pDev, std::string file, float width, float height)
	{
		result = false;

		if (pDev == nullptr)
			return result;

		D3DXCreateTextureFromFileEx(pDev, file.c_str(), width, height,
			D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT,
			D3DX_DEFAULT, 0, NULL, NULL, &pTex);
		if (!pTex)
		{
			std::string texfile = texturesFolder;
			texfile +=	"texture-2.png";

			D3DXCreateTextureFromFileEx(pDev, texfile.c_str(), width, height,
				D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT,
				D3DX_DEFAULT, 0, NULL, NULL, &pTex);
			if (!pTex)
			{	
				//Todo: move this to imgui
				SafeMessageBox(HWND_DESKTOP,
					STREAM(texfile << " not found.\nRemember that .jpg extension is also allowed.").c_str(), ALASKA17, MB_ICONWARNING);
						
				return result;
			}
		}

		if (!SUCCEEDED(D3DXCreateSprite(pDev, &lpSprite)))
		{
			//Todo: move this to imgui
			SafeMessageBox(HWND_DESKTOP, "Warning! at special level 3.", ALASKA17, MB_ICONWARNING);
			return result;
		}

		result = true;

		return result;
	}
	void cSprite::Draw()
	{
		if (lpSprite && pTex)
		{
			lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
			lpSprite->Draw(pTex, nullptr, nullptr, &pPos, color);
			lpSprite->End();
		}
	}
	void cSprite::Release()
	{
		if (lpSprite)
		{
			lpSprite->Release();
			lpSprite = nullptr;
		}
		if (pTex)
		{
			pTex->Release();
			pTex = nullptr;
		}
	}

	//========================================================================

	//Main background texture
	ImVec4 colRainBow(0.0f, 1.0f, 1.0f, 1.0f);

	//Players textures
	LPDIRECT3DTEXTURE9 texAxis = nullptr;
	LPDIRECT3DTEXTURE9 texAxisVisible = nullptr;
	LPDIRECT3DTEXTURE9 texAllies = nullptr;
	LPDIRECT3DTEXTURE9 texAlliesVisible = nullptr;

	//Weapon texture
	LPDIRECT3DTEXTURE9 texWeapon = nullptr;
	LPDIRECT3DTEXTURE9 texWeaponCustom = nullptr;
	LPDIRECT3DTEXTURE9 texWeaponVisible = nullptr;

	//Map texture
	LPDIRECT3DTEXTURE9 texMap = nullptr;

	//Misc
	char texturesFolder[MAX_PATH];
	bool g_bWasInitialized = false;	
	bool g_bMainWindowOpen = false;
	WNDPROC g_pOldWindowProc = nullptr;
	HWND g_hWindow = nullptr;

	int m_CurrentPanel = 0;
	ImVec2 msize; //Menu Size	
	ImVec2 mpos; //Menu Pos
	/*
	ImVec2 hostsize = ImVec2(0.0f, 0.0f);
	ImVec2 aimsize = ImVec2(0.0f, 0.0f);
	ImVec2 espsize = ImVec2(0.0f, 0.0f);
	ImVec2 d3dsize = ImVec2(0.0f, 0.0f);
	ImVec2 miscsize = ImVec2(0.0f, 0.0f);
	ImVec2 keysize = ImVec2(0.0f, 0.0f);
	ImVec2 infoSize = ImVec2(0.0f, 0.0f);
	*/
		   	  		
	
	/************************************************************************/
	/* Styles                                                               */
	/************************************************************************/

	static const char bank_gothic_pro_light_compressed_data_base85[34530 + 1] =
		"7])#######Q5e0Y'/###ZE7+>'wPP&Ql#v#2mLT9EMdK-Ra+##:G###J,0h</pF8_3g###OC*##kK.e=6Zj$`WS)##iO4U%IL<`<?.41Da-34#D<F&#<;isBb<xeuZ/q.#Ngk&#W:N]D"
		"DMkmr5_,oLH5P$M;$tLD6q;W%j&;*Mq1N*M?O%KDDTf/,,j,)#w%D5#e:V)Fo;>J#Yg3O+5O$iLoMbAF(('[J'j8*MPw-tL2,%4G%?c+vkG.%#p&3<#C9F&#/k8g1VpsSpplv^AeI1=#"
		"?<5)MwKg:d2PZYNm:kx+'nE)<M(aw'K>YY#t+9;-kbrE/5Lc##1HdtL0aUY$pt8kMBuCp.nT%##QhSe$TAlY#g0_'#.M(Z#DY2uLr.BrM/lU%kP:c@tp#Z%._=4,M+:9/L=,GX7E$AnL"
		"EPG8.L#TCHIC9?-mt:/:#^'LGp5_qD=HSt#Iv.>-XFE>.ct65/Qd<X(all-$xHce$AP'588=(^#AB<)=1:h#$RcHY>2C-?$hb+GM3LHZ$u]g1T4Udv$2:t@X7q`s%F>#J_8$&9&1$Hoe"
		"5krS&Jo`PB;HX2(8i]F-H(u_.+MC;$WaEF.#^$0#.(1B#ML-Yu01L^#[nvl/1:h#$c-KA4.+_>$O?IZ$'QfrQ@$ev$Ma);Z7q`s%vi3Mg8$&9&K.u@t9-AT&&hrS.>Zt2(D86#>Z[/Q("
		"NafYGk^T%J$'4RDCKlmB&8taG4sOVCmiAoDD0V6MHr5/G^SAO;:.C,38$ik1^DxkE/$A_59rCVC:1k(I^hx98)n[F-b1USDa9vc<*5s3Mmk&kM$2^gLPrj^-3G7]0)ir]5$&r(8W6DO+"
		"(;1eG38vLF)-7L2)NOe-Dvk?Bj5x--0]6fG=9OG-c7$lE^=N880oiiF=i3nLBlSE-%O;9CuicdG/iCrCBHjfEo>pKF+(rE-&a'kEA%o'I9g0H-7PViFTqm$8X3L'>=O%F-FDSE-4#?lE"
		"S2&aFewTt1S6Xw087M*HBsP^%a;,FHr(622d8_DI@.vLF,v_aHBFwgFXfCP8XhdxFeBEYGIYo+DUw%;HlBel/Zl,20j'(m9Ls%aFpB7L#80(@'?Cw?0sC-AFWj.@'YYbYH-&K21/HnJ;"
		";X2X:[2g,3/pYc2L9*j1t'Bs7i-7L#IQdF@NMWq)sc8JC8C$^G%?g'&1G`f1rWu1B3`SGES'm`F8]WJD`_&mBVBi]GYc[>-$6nwTI<Pq2x'AVH'TC5B&aS:Vtk>xTGlv]5E.AZCh`t3t"
		"i-?Ra0CDqD5t1kX[U'XLrRBe6J]/kX)9tQjY>*m9UVX>Hg/[-ZNwmKP7`DJ1pG&g;UDf]GomS9rmg3_]2vUk=+]+wpq,.wp@oNe?/_ufDd1o>-+JM'J*Xr?9a&s.Cor3)FI&x]G$#w9)"
		"qcSfCZl%A-GuM>Md(crL&#/qLIMg-#T$S-#)EHL-Tr.>-S`.u-8S#TMRM4RM;NM/#j=$p-/W&E>#mGoLUkFrL7KG&#'9x/M6A1sLVe=rLGAwx-750sLa*8qLoLG&#.=QD-d?:@-Oh;H^"
		"Z3CkLwXD/#^st.#e,KkL>?1sL$f`/#c5o-#KT]F-W(A>-=NJF-mSR2.-N*rLK&mM*Cd&Xh.a9<-qx8;-%GpV-Zfxj2h2#&#5(KgL[bv>#3BFV.$KA=#FxS,M71T,M(LsS%+6@D*TMw%+"
		"XfW]+](9>,a@pu,eXPV-iq18.m3io.qKIP/ud*20#'bi0'?BJ10U?']bpYc232;D37Jr%4;cR]4?%4>5d(=ZGE8E)#IDW)#MPj)#Q]&*#Ui8*#YuJ*#^+^*#b7p*#fC,+#jO>+#n[P+#"
		"rhc+#vtu+#$+2,#(7D,#,CV,#0Oi,#4[%-#8h7-#<tI-#@*]-#D6o-#HB+.#LN=.#PZO.#Tgb.#Xst.#])1/#8#krL_U(%M#xA8#F7I8#kRp2#qNB:#HCP##tVe#M=_YgL_w$##F>(7#"
		"B2l6#vuO6#dTK:#tHY##v]n#MoRu+'t&,Vmh+6PSR<#Po1m-,)_LEcVj<>]X+Q7VZ`3+JUBbr4S/ahf(*S%GVf=MiTxpj=l0@TxX7aPM'@0,/(,sj4]hfi7eLcCVH<A`(Wk0(Pfin+Se"
		"q[BigqOb1gLQE>#tv;fh#+$JhR=w4J1%xCW*LZ+ix^=PJ34no%=fr1Ktc6MK##RiKNLt=Y*V/GM,cJcM21G`N4=c%OZ+_Y#<n$>P>$@YPC9[uP6a;YYGQ<VQKp8SR5@35&;0A;$%o&ed"
		"D16;-Q&.kX/WJ0#VYQC#f$2G#TY-K#7U5j#uapq#Uv@=-FZF>-V5:?-#YEA-Uhoi-v*w^tRle+MKKG&#H'wX.dt(G#YW_@--pO41rT^q#Rp7=-BN4>-q]_@-TgX@6$,Y:vp(>uu9f-4v"
		"+a%1vM*T.v>sxXu`cGxkplanLn=8uO_lZ=OwLZ%*bBCr#B$E>#`)bJ261Ki#^Eg$'[Xq]lJu`4oBbpi#M6TO690?;#9UK:#m8u6#S1@8#B2l6#-Sp2#/rG3#g,EQ.hmGG)DkYf$PJ:;$"
		"'2G>#HsABd--(V%<$UI(GnvK#SF?$#C&A`sbxKC*HsUo&+qW<$#.-uP84'It(m*2(9?FM(/^]6,*ir$#f-/]%OAP##FXI%#q2h'#ADW)#eUG+#1n@-#V/:/#$G31#Ke53#ov%5#frCg%"
		"s_Ioe.34]kUq9fq2I82'i?320L?.29.9)2BjD?MK27l%OJs]oRcXNcV%?@VZ=%2J_Ua#>cnFk1g0-]%kHiMonaN?cr#/Y>#;eA2'SJ3&+q^WM0L?729+t55ATUZ]F+l=DN]Ls(W>4r+`"
		"pkPfhQROip(V$W$Y1P;-8fN>5dS9,;6Q;2Bh2qlJIpooR&HnrZK$X`aP;Cp%(mSP]kwViF-M%/G]9`9CxdM=BO?'2Fk^iTC==^b%BxXVC/s>LF=/2t-JBnRMf7ULM6m7WCN)]'G9Y)iF"
		"-]AF%aX0pM2KRSD<hZX(<n3cHV.]u-D2'pM3KeoDRGVt-Ja*sLWf&Z.m&4RDPPVX-uk<=:du_3O528F%68AF%8Do'&8A]b%6;Sb%68AF%8Do'&9Do'&7;Sb%/NcSAQ%dq)ILwwBP*^UD"
		"V^:4FZ,ehG_YAFIe4(%KlndXLp?87NB*q/D>CRFHAKi<Le4`e$f7`e$g:`e$h=`e$i@`e$jC`e$kF`e$lI`e$mL`e$nO`e$oR`e$pU`e$qX`e$r[`e$s_`e$tb`e$ue`e$vh`e$wk`e$"
		"xn`e$#r`e$$u`e$%x`e$&%ae$'(ae$N[tsBMB7nM*WAeG6bPC&6%VeG%^4rCH29pM>TnoD?gXO+(^IUC-mgoDTii=-o3I>%O2LNMVWbpBnG^kE,(u$'oowb@oowb@t7Oc@gsg4=I^qR3"
		"I^qR3N&IS3Lp6S3u:clE:H7RM83%.Oq9..OZ8..OvfabPgAIIOT<6D-q36D-M%qk.XJcdGP2&Z$(A]F@(A]F@-`4G@vDLo<N*F%0AVXo*),E.?.dioMu6/PDIUaqDA(:oD_NYs8Z@LO;"
		"cS8LWqa31#/4>>#:b/2'X:lr-4.GS7khEV?fvt4J-xF`Nt)7VZ:xsu5UaIM9xE08@aAVfL=q->P[UJ`W2/t4]]Dr7e(YVG2OJCMTv5erZWscuceF3j'rkeY>TbIYl`E6Gr93U,))^(d2"
		"j',p@tt(vP#9[VZO6'&bkuRSe_*(/qmwCs-FwqP8&4xY>F>J/Cd?sYG:UUAO$NaS[fXLGiJiVSnjjdcr,PCZ#Op_,)QN[29/t#T@TIH&F.PjJL,/G2^ePiVd-U;,iD,L>lmY'vuB$p2'"
		"jh:W-85Dd2Q$Qs6;T)QAZ[Q&F3F-^OdCnJU)3`>YInaDasb,jgKYmVmtcO>u;:es$LNYj'p0%9.9JIa3a&o29((A^=Aam5AlpOsH,8*NKAq6^Oj[<gU-E.ZYE+vM^[T0aasItPf4kM,i"
		"KM?vlo/aDs;4RW$K?,3'e:p#,1W#01G+4B4ZE)97O8K=-COW2*#PC<-@QbB,rcWS&68^b*F+7=-Au@T%/AXS&`TeA,7:t9&]tB;&S<N_[wkB^s+q.7ng_`6nbrr8&-i<B*S(n'Wwa`]t"
		"Yv7@:X.=LBw$v;dV2W#R/Pe;?itmf_,rWX52eR/QwB@S&@f9gU4hTWLMkmS&k]*ZL30:as$[O_N2<;<%/1x_<<*^(u8[lG,(GqA&#tGWg5(oF):3*Q&C1g0qf@X_[@S>/)f&;tVBkv_k"
		"I?;vgG@<i0pM<^8qb,>%8VMOak)5hqpixqCcA<<%,:;al3O_$%I]6HgiD=N/a*a_FO<Jf,8$/^FY`e1-1n#^X'dIl,]8pPS#-tgTm;(4T+:qhL/CdwL3=Lr&'2_e[;'dU$1?+xC*?4e)"
		"1fIF)3nFXGiU*t$,h8BF@C1/)l]w-)SXWS&*wln&6E.1(]iu8.9RF9%,F+HD$EVQ&4qE$*W&_(uVJbxsa8wU?LR7YD8#qHq:xh.[KuFH;iwnYD,YPK1=MFs6Y1?bsLIUXu)'3Xu-4K%4"
		"E2Z(@=__*NE%T//&7YU&W$VHMpoW_SGZ7vY=D42-=UCeM9ehD1<Khr%8oE,]U*uDss<Vb;Bt-gk:h7`t>+,Y#iL=&-U$##[WG(GsC.R`_ni<2-AJOjt1cA1KE,/_$`wL[Fg7l0-)drqI"
		"hZ8N+IR:QMJG'xX^_JiX@@,Q%?A_Y`BXK/%V/q6D^3((*)84pe[t#b,-^rMB]NKnEEx*PT):[VsXR+bduH,ft6Z:x=^?p:%PZa*lZ=v(%SS5Jfoahq)8)u7DODB'%MD:7X]h[5SLbAQ+"
		"?*7MQ9=%4A4J*eIJ0gZPa<dg,j:=ZKFbmS&DCx8&Z[^3Eu%voA*VCd$w`Ei&$<Au,g:/I,SRqk/0uc#YO5JPsX?iu?</]@OQTIh^Uw;Ws;>.nj2@M#u_v%u,K:],)Z3B(T-5WFs7FI#u"
		"gL2`tF+qDseoC_tkf+fC<KX;HdN:DI;9L_WnWNGUY^4`t3/a-_>fIk4;U#xUw;DS+$?4`tUbQP0-i_d$u0PqZ0D.7n'NX>Gcg?&JupU;.%E`5SbS+X$CR+]s<v510OAkI:c^pw4Xd&di"
		"dLo_,lxL/UwvZ`s4#,`E;Ts=#sa$%%?j=]AGfQ8%bOO'@FeK/%O(-9WqT6)0eX=Rb$raFs:i]X#b[@7-SJ&f)1_aADB4:`tU+oSsXGm<cU*es$@Pti$PJdGVr6A+-OQ'GHlG;`tU7aO<"
		"/ta*-38?,MVP`']M_L'88h%?5fl3RD_O^.(B^ZG&ONtw4@D&3Kt&/b@0`mr>Q>i>>rf)D%Q.5pVV$Ku$K^kWVkBbcOQcR>I1Pq<1nRiD,ch]>G><+,-wG0ZGSh5l,e6O`6rA(S,:L<R,"
		"k2Y%*rr$E3Sf>x4fSw6[>c)`3a%wh$vp1;&jBEC,_-GJ2HUNmf'ZtFsu5M`sd&UnobsTh$I--1gG$_0gZ`tcidF+Q%PWMACtt:4%A&od)6U`#Uq3mI&]UHQA$)]HDXv?8u_<).h1U(O%"
		"3PNF&3%Nd%Y>VQ&jn14%/Re9mEL8hXncfS&e9L]jaovh$&HI#uMM2`tol1asH]-0uH.:^4sk=oLr3D'%BWWc4#<FbsRQ4^CPLm.5*(+qCdJ`%uOwg^[V[&GspUH,]2]n]5ep5T1U/S'j"
		"1.gtYAg5?Gh/Y-H6?^-Heq?I:#XaJ1K7w_EUa;p74sRht'-4kO%[IDsA@mc$kYB_tp'TKg-#6mSZ6lj,DH@as<le056:%#upn^+jm#x>MACb70c,0mto#iRsE(,Y#c#,B&74IaFF2C_a"
		"L7VmLtk@iD/(p*u16nd)hBq_E2vVnD=6eI:1^q]4J6*Q&5SVQ&L/c'kAv9=>q8N1B(]lJ:CuBZ>%(677Q_<%*MBMDs.t:o$$:<A&v,&EsFYO_N?6cF%$D1v@4:r*-rN?iLWHR8[F[YQ&"
		"ldu7SMu29IZHOC)?@4/)lW>Ao-MHR&rs6as6dXipEh+)M$-CQVUgQ9-9d76IrDACj/?0j&eW8D,mB,4TNoPFs%5^u$vQ9EscDi'W;V]S&1tY9&c.t(=[FGc0O5`-I&E9^&R@d@&dNtw4"
		"c#uDsG:Q108N0B%?=Y]t)nl>u,QTas6[pY%o@-V-2M,bs:.9*uef)r-1fC$uM89asd&A8u?&u>%so@DscE@P%^];o@#(4N'wPqwsOF:FlITvd)2jRbD4r2Q&_mww4Wpmk%mB1iX38D<-"
		"VR<I&`WiL,3K?[sxrFaj6Wn`sfDGn$-2ko5^E.#ujPGn$#g;Bk[r9i&ZuTeLP(B3tJj$ZsK(q*MA7at([u<[s7_ZfLJX/77qB$AIS,#5nbj')M^QSds_FWZG`lcL02*0LTL<gxTVpc6-"
		"OZ5.;SNY^tr%L195'4`t>8lSpd6AX1jMl6<;nU/Cv*V*-KPBgho/U+X2q%P%4J;pV]FZ20/L$cDgJCh$L];^kjnmR?b::j&D),Ze]eQ&)iL^4E$[T^FK0Jf,Ou)9ICs:7@)YXt9'_QDs"
		"xLuxtfk9t6[$#J(i=oT#_[`nItT+%bfrT97[ZBB%S?W_[[dN5]dD=e,V8IvRjPCD)UX<%=]3Qxk6tCUQL5ikojMCC&+p$lbRB]O9r-AQ,W6Ia3Dmxf_38v/YdWF>D#p;d+*G2Q&F;]S&"
		"qdaX#e$]eg?LOs(?0w3g`d9h$lhdXh):C^`HZ&m%;iZAIELY/CeNVm,g1S8u^0;Q&TFkP&Qbi.IZ*rw=E1GljANEM,Zn'U#c)oh&,H$k&.&^S&)E>Q&a6RT,5o47$eSEPsh<e9vZ)St:"
		"SR#r6SPiERd@<G]3qA7/k-8Q&;s*%nah.T%ur48$3FHHbWDs>ue`V#lLBnd)XR@o.pX.SF(Wo?P8+:K,2FNUJIX0k,^f&7[f+R4A$wCPsD5p%7+ZBF2rGoHs#5QxkIAVYj@P+f&.#:_&"
		"ZFD]&&/LpLwvU%kke]IC/8,7[W5o5JLXLTf$H[7$@8L/[[vkP'<_QMh)Ltw4udivshGT?#3g%Es&_T?#iI(##WmYxOa>T2#9h)##rqi=c[w,;#lR(##8-Juc^-?;#jL(##Xrj4SrOD4#"
		"xw(##iw$GVub`4#wt(##=71GDdJM^4JD'##0C2VZ(C]5#+@2##F(:c`<gh7#A**##d?+VdGMn8#E6*##q>vLgJiNT#LK*##$NV.hR=9U#Td*##0D0`jW_^:#Zv*##us`xFpC24#0N/##"
		"S=x:HA.:/#d:(##t&5YYhi>3#KG'##1Z#AO[v&2#kO(##v-KlSYjj1#_.1##2(6PJ.g7-#f@(##H8h(EGU-K#'1)##>:qCa]&02#c7(##rP^%O7G4.#E5'##mlQS%n7v3#]&+##MOSiB"
		"VTE1#<#B##aI=VHIX$0#%+)##-0PuY29hj$5_2##NLQ%b>mq7#`.(##*0+GMa2,f$6dA>#X*GJ:&),##BHLpLo#q<jE%ZP/YG#)<`Dr+Mb0(>Yu/PrmT(cV-EF7;HQ+s(WeB]ulgS'Z#"
		"bPurH@]@PoM.q`Ef^08n>w2j9`wv]F9eQ>Phn8)W@.VJ_m7XPfB8?;mk)a`sa3Aj0(_#T@gi:)a:I1aN63]Djks;)s?<f,;%#*NKO#wSe^,'$#i'ep.=.w;Hai&Qfm//as=t`j974l)N"
		"w>s8[)nW&kLaqg(#R[T.J@'$5F&jmA3bA01<EZT@jZ2<Z4PSjp8u-[#%9cK13[kZ>W&B<Q&3lvlpU,_+5:F6JUR[^kmi5h:J6w^O8M(n]ZhTNp15GBtf1#C+m%;e;l33bE5Y8kKiruTR"
		",$D'Xr.O9eW(jWm;43=$xnG_+>;i-2D>'LC-SB-r:b1kpNp9I;ZCII;m>rb3%v949AZw6f)Q?.27g=Lhm<dRAP<8CtN@gC4rCI7SbNTI`P>HCk)kJr.wafCFd>#]P;?D+WwCgUe)NSIr"
		"Dw7S8Rpm=?5EeCX81SY$<N^r.XJSr@F%KoJ5k>iUo2mFah@a@lrb_`+)vBS8$X/GEM@p4KBg[(Xr?_7of>@5'&xS`=n:,8A3eC2(&*/AYf2LDF_HIJM7XKPTc_MV[6So%caPUfi6WWlp"
		"egGv$;k@&,htB,3>(E2:l:cSAES*vHtiGAPJsIGWw&LM_Klu+sYg$a=30^GE`g-)bB8giqqJ#T8*^V;Ivc#aO=T1)k`7f21^#R&>,$E)X$(p]lMlufrtcep&g_k/2j7ApSj[d>Z2#N,a"
		"gJlMh1$7sn/F[a+.H/B5P'Pg;@,@^GjL+^ljWKB#:IDH*0)hs72-5HELA^&YiXmk/h'i'tEkBBu+7&I_$oC_tcnvk$)eH/hJ;JF22a%htWKs<Y^tZ$u,M;.LANmMB^,/FMx1[(u_D9Es"
		"D]<[s?g%4J(557e.0f3%D:Sq)&IZqQ'B@Ssjr5HMdA,DIv,,#$7TL[XS.A^b.J7I@l5`49_uCvYZgI$tnunBjRa%Si?ko1hJUjL%aV25WB2wUU5kxJX]@,TVQJn4CeL&*uV^MUmnx<FD"
		"$c_$MuVkP'=&&:H%b,Ns^B>rt.p=DswNhtu`3T%[<T%j$u;:N'flNkSoEYk$6=Y>@n7bw5RHQs60O[@%cTZ]trNJNgX@[egYW>D&W@b]m/aYn64ES%#=$[tua[+[lBK`OuiG3-)-W(H,"
		"8]M<U]_:dr=lDgC^bO@jsN;=&>N._s7q3$Sp&I3m=[l$Oaqh<dR,k(?BrpDs4<ri(DVF<&IwxNoA?4D,@#6F&nf^PkZ.g]]#=BMs@M;UQUilI:a1.Cu$wkh0[Ihet(5-.1xc[RnRwG3<"
		"l75=M]+HD-[ERu1gMaLudC[L,],J=hiO;'MJr9ba4'P^%AZ&I=?2LaO%B#De+OF2X,G=C3HF+GexOKR[6)/^tZWWiCSSW*rOC%.KmxKO/>]^4e,(V_/`b+ZsHeD_/R9OA&M-Zeswb1s^"
		"trpqs<tL$tN6SeCFbo9ZOq%i5J;DR@qs+Ai8Oc7c-r^s=-h[FgYF13texnDs`+4-)^(u>&J&]&G;6(rtd20_-mout1mHtt1G6.$tN^=?->2hs.K3TN'hDj--1H[B#qaL`sYY`g;mxpk1"
		"FXCPej>u$*N%EdtBle8uguaj0C,a0go2DehcfpP,wB9`5:i1=Ikx#VsihbC3t5BF2au]l`&v4L#I$'+2RWCF2[8p,3iVSk?oS_V%NaG(3+LI(<p]x]?X[l7$l@-%=:P$s$G>&e$D*Not"
		"-BbjLqYX2c(pj1i(8,'tv42^td=)SX0LOLs;(]_3$j3Gi=XvNFJxYk]o(XH&q'YH&]e>*7K2vPJW[9)NvEHit,,K2%Kq.]$%c?vGxwDGWwx2,srES'MPA>TsMJhw-L]/.MX5W:6LuJw5"
		"B]CP,1dvG&wVY%*-C5;&xcfwFDpqx$mgf]bkREZ6(P2etY]Lp$Q+get2+sJk1dxc$WclDsKr5PfZu]h9TGqwkimC.)0Q<,<+]r58L#F_N5<A_t,#PQssD9Esnh2-)E`Rea=d4cs+SAcL"
		"h+;RY'7eRjTmdU#jm0qkdXT7$3BJ_tXVELsPG7D=,WHiLDUiUsbHx[sHf&:%:$FSoxGM8BNvhTC/Pg=%`D-Z^^80vLeA:i'F@8?K$Hm_tsMdc%Lu9#YL>mqW``N]sGkjX$W5t'<,/7Z$"
		"/Q6:6V8CJsCol#uYw+w#RJ,%=4*m#uIE%2t]4w=cxvY-hcx8i'x@TO&gsrer%%m%dKlQS'7@e)eelT7BnX@`to#e;%]u3ut$aT7Bt+IA-L;e]-/Pa-6J/(F`vNH.3wMtt,'mpm,?8?Ds"
		"_E>rt=u12MWaMJ<5<4Q'r]gvLjuBh3'^$u,L^:P'RcOI:i,9KIec<bbNu3xsX1Cj_SgALb#N&>qdX6sQT-_uE@'&?/h]?xnElUZ$j$X<qq_%N';'qsHRj#Zs66qx6T&M`s-ax#*TcDOs"
		">eo[Fux:Ks$1I:hIq8N'A<:+M=OvwL&XDs@:s6&2i`0k,wL)gt`jQ0%r4)[$n+OcR+TSKU+@@OV-AH.MN&Q+Y#WQN'#S@777#)5ujF/+c='=Hr,gM)`V/2@AAo_P,m,38$ApjN,@epi("
		"?]9N,>fmE,b-rFspwQ(tiYpGrSPHXP(^KOJ.&CwO$8`>lt=bp:HY[p.7Zk+`wYvB7''sM,EQi%tj*sGrrrsFLr^jr12L)M,'bQ&twxKnN3KxqD1S_;$q:b6ak<WeD&mkU$]B7H2.3DUQ"
		"fbHeiY:=]s(/=]s>:DO%YOJ4A,D7N'O[:xfRFOT#q4mb2uG-=P@GlP,^iZX.nSf#*uq%31f/%W6YRTut6`Tut<1]T%$6eOAG0rlkJG>4&sm.:Hb_(Fsd<.^tT9:>6voq^sOm/:%x77f("
		"WZ6qM4gm_tcmaU$O^OIs)tvrWiGrS++>Y=G%E.h%D3petdI8hf?/vYsG([3ev9sDqL?)Yh7'i%t-Y+Fse_;=,u;P^tdDWILq%0i<D5a=,L/0B%SY2rtAU4c,`B^rtk(&Y,aU*u#H#0B%"
		"[E5pkNBwFsjeVX,bsHu#^9Mf6j4_ER)Et<YBEGLX9F]RRkh51FtF[C%/J5,`K`0Y#rwFmA6R.+-M?7dD_B2m,b4LDji)l<V>)kiYF[C%lS=L@O[?5nE>U6h+j',gLDtb/-3-&gQaS%Gs"
		"=YmaNdqLUQJb3^kZit-LE%PLk1I,.:[2uq?@[#30HvV9FFAfQYVr&GsRS5asN]$)4H8c_FP?Jf,K$.@F*GhPSs0YB$jW%@'R))@'Gn9%U9XiP'Ip0G,'?j>&Dvxnj_WqJ,`p<:vt]QJR"
		"tX<^bp%TL9Lcl&FaS;bid+iE3`'$)*q(@Su(G?biE*Utt3$ZC%ucvJ1?YXhK%gx4n)nQ8$e:P-Q?<74foG=itZaxlsA'1jt[Ko@WGi8;&F/GC,=]>kFC[+L>,+*F`fDl@2*]i%tio*%="
		"akF6u0E>X3FfG^4m&abiv+=o7/G38$d9%%P8u?4k>R-K#G5o<lp.$@bdrNwtNN8xt'*8E%#*X]tdclDs`S1KUZj)+to5gICIn*(3Sv[]tITQ%+>hCDsVK7/4G`348kn*N'vtf9v>]qFs"
		"G6r@l=oe<LwLf<LE+i]tuApet&/I`$4LDFlq_KDslZ3Lsux6`$:(8'o-x%=1/F%8$]P/+NutH?#vZb%t)4*:&v5:G,J@btYb^0N'2:7R69'ZnReKQ&t4%$)*S<#F&wF3JI]3FatkdMaN"
		"</Lx6mX+pe)x()j>Kfh0%*P#>h/vv$:=u2;(f@o.Qh?uMD@b'M'IqSk#Mq6<6]H_t1vgj$R$8D,&+/L>=$((3$o%ptW^ums`u%YKxfV%ugn$os0/<(Toe=2:hE0j(1nTT%2R^R@nI3-)"
		"OA3/)'kpx>*Ve0^0FVI1dnMbj*ovIsd'`:m1dYEnIAb^Oq>'SI53+w#[4.V5;`c>&+%.%=i[_]t;47st@wfm$6;@nnn-41%U%jd?B=VmLRw_E4gFa)ubh-gs3Y[%%rnUI_kL1I_XCGDs"
		"C):Yd^f?gsPt2)uF'B2%,vg6<s&#2*8*/r$BuPwkSL_]t6?vnt=,Oi$sYEDsWaTut8Lqr$G[>XPiY&:QMl)N'8(nDsiGYf%^d,%=RuNOWLIxe(vk<SSjgnfhDic0)vhgE8ubo,)H'51B"
		":2eAikfBBu-:ed`2e[$uH3gI=uIoq`X8BN,UV+X7;b][s87]'txh;k:M&l?-2ujX%@8Fm:$ungs6xW-qJf2L^;X;_tDGPn$b%%c;5.5pG?tt<YC6C=YI_6%FD,MR#]E9f(&t?%O5QTas"
		"XOMA#v'Z>&lQX(jqHa8&t?d=CWkKLj8n;E<%f8csWK%6M)SNZsvi-;IK3ZesYp*%=kE-aJ[<^%44VA2p.?2P,UL/ZPpXQ^t']-#$R_4@c'ik>%uO[]tcQaM,$xEdt%3:1'Da9Zs@VsGI"
		"'PK?<nx'XiG/_3p+Z0#t*`n*LktlLF6NF/)rEm=Mh>4i+8B7gC'=r*-'/pDEgs:YMOAHaI5@T(FPk<$LJgb,Hf]_1CT-=;EX1.%+LH/.(<q8OWb*M4%PKN<VJ;;nX87^sLOC/'-&>-J["
		"LWib%b%o@+N,a1%TJ0_J^_&TIlGPg,;YS2%;n.(TP]JCSLC]m+U_hwFkGdHLJYnq6%W1N'X:rFscdTO,T^LT%$5qFsALP%=74IaFr'p*uFA&nJO8k:_+w:]c%LOG,^.Q.1>G]NI2D)>n"
		"l?<O^FfX2-.Wc(u%qN/Uwn'(*hdKG,Fg;K,Jcw5/aa.FsS7&I_?j@Dse.MUmm1?_arh(T.jVpmth%FC3L<a%YO9+oL,vmt51F/FM6XiS%@v(fs#BUO/(4tX`0]aFs@JIJjj@K8[i)od,"
		"JAuY@5:r*-h/OlWA`shDlxE,ML2nlLFubYjg;VP'9Zk(Nsf.9&Sp7J&&@O46ie/[sIj5EE]7qSg(_oJ.v1+)>f)Cm/+3RDsGH1^t3sJv.`%2Ds;`0tq]Z7J0Ippg$<st&=g@5N',=m=l"
		"^bO@j4lRN'^7lXh2pUZscKrCsZ02DsfA=j)rGoHsZJOVJaQxn/HLv$tDGpFs&kSZhYDCJshUX<6Z5421-3Z=5d[uBac$7osCgox$(S[+)$;7+)]OGDs:%,f$krqR,@T3FsE`>A5v*1c)"
		"+U=f(wVCv>7+`$u.P5bskkxx$cwZ$0hOLDsp3q^s)^bq$PwRnL8M_'YH#jOs]29Ms2*jw$8qdI:4X?I:LuFDsSr?kt%'`g$Ev1:6ZE177;4&N'rQKP8(>6FG[AhB#QBZQSuS'hhe)14)"
		"KlCJ8Sxo?%[8HuK)>/Gs$EoQG1j2j,p4t-[.X@['>/s>UxKBnf<6Ov$Ye0_@027H%FwFgGT$A1W<rjEJTE['4Im0q_p)C4J21)Eex0ur)31=au8(e<CQU2UsCGrB)E`TUsH#%PH](V;U"
		")Do3SY(0%]_8.j:2YAnfLeKjFi-/E%Lvg6`QZ%QMx+0K+QP=bFUBJf,Sh9n:.>cQYGDYn3o`wxU;HbP'<WKUs7V&vH5IJs(L-vBjGc=ps&FR&*H_^@%ovUdHZma]tQ7^,)&vE^JGnd(="
		"`+bRDRSNnSQ?RM%4'ksV?YKDs4Kf--1kJwTPR4n+O(PmY]#lP'<NA`rQ+hasct.-):+NDunTA9&e,31'5*1R[GSd4$k<I_tmlGZ$K^KtVSmQ?&J@llWHBU[kUN'F,:L.%=B+O<ut_b1h"
		"Cu6n=D^r&HV)Q(@vI5asDj0-)h:NT@h`tu@2i$A&+%-7R19NcsMrrM.hmpXsMRJ#*O^[D&t>kR,a(XfiwreJi65Dr?EL[<-QX_p$nP)vZJW4?-n[)60e<fG5$r1as'fUe$b_WD,Q4f6I"
		"v999&53Vn[]E,h10b@A&Ndh8&>&B=#%c'jLdh:ZF1E6[s24NwlIsSX%D?fC,-Ai;mMk,%))#>&M+c<Yjtq4tL,<p%#.Q:@E$tq^f)e8csC>,g)Y[OTs+)H?&r+SK<[IWE,T(2D,&vT#?"
		"Ax2P,u&Xg2r68@=^RcdM]65B,a1KPs2YFYHfhm-$'wYmJehKhLeRTA&K:Z2MY7>HrXBOC)27B6sb<6].(8GM,B6>gMrqs=Ej/`&P)dxA,`xA$g..JD,>f#m4L4mW-Hf&%'v[ID,'Vv=/"
		"'8eNq#6RL,>?69&rhTPs42=X(/]>X(I*LX(8nx3Jiq_d<ajYc<RBhs?T>7Q0^(De7u^kKPqVIJiU0ba%==/B,vMEXGW_XbDVBfqR$o:7Us;qbX8gsl]'CE4&vv7]$LpCvs_jt$=i4xmc"
		"53-5$%/DXc*R/<fwM@6:ZH;E:,O*Kf0*S1*?P.*?QM9Wau$$.$6x-*?P'*h^u/-T,`#qw4gm_#5vT_7%>r^9vALd:$Y`dh$/^O#DO6;`tXQ8h3eWqMBuwL*-@F>jBeWim,d=+]+RhYFs"
		"fV?g[K.G`t/9@_tgRgY%RC-N'3NsFs)7R&teCB9%;_)`3qn/PBgr3FI7=KMs*'Y@FZI-j^?1Cn,_3TtchRd]kBWa;5a$D4%0*T]69/3N1WsaFD)%k*@Gu/b%t@tqapA1+)gZ/HKE4I/)"
		"=m3e]ZF_1UB]H3hvrB')Y9p5i7W#7RdP0C)vWt]saG9M%r,r(B#)_hpNMmonDnt=%&E'_VZe6m]2j]Eed:uMB#xC*-Deb<m%K3a@PEo@%8^G7`A8]3o=K$gL4%KstT:j87Ao_?%vFkqL"
		"4wSI]8lCMZLHrn%tEGwU;`Wu6eob;LgcLh,^F'+rK&:;mOY`>UkS%Gs]?(_$-eUeC-u%NI6>JPsN).J9MmS79M^57gU0mIgYYdP'Vlj;te_uCssJxocJ]n<tg=f8u&qP;BQ,D`tVuP;^"
		"Xw&GsP#Ott0ov'9o8C.5Sk8:%QJ5g)TaSVe30;iOfpLehl]K')='M'6Gs^)hP.Cno15g't)UTJ%L-Q,Hq$JvGYaof,7[aJsa54hqR@B+HXA]vLRE:sLUK3s:fv-_$N-VJhmD+5-g%hJ_"
		",J1@RNeQ,_v7MOcS'uYcsnTa,2_3mS[?p:%?B+t7RaHr60ts[=)^pWsds(F^`Ti+``LhWsL>L`sP2*m)<@vn?&J4Z,MMCr$^.x%-^>Ai`s;4Z,p7uq$ltR7-D3;ODVZJh,fEX-cD;NO%"
		"9i?%iO]:dMFwtO;x>=o.Mgen$[-&[UF*KYd3[a;UDw;u5qn3wiK9sn6Ztfb)xVWRN]3*-a4FQiP^KjP'@Z[Ie?<p*M<5)T#;Z>2f)#'2nm]KOJ1'*FsEOfe(p.i_tZ-cC3O+%?7EK5f("
		"Wq8:%YIxst=JMa,KFGDsLcF@+;/>596up71/,l$0>2L=oE^$I6:&PIo'^Sb.(4b^br_XJ2N[BXdk)(h$bOTMs?X.W9@W,h$a+)lGPW;`twnI.GvcD`t$afLXT:&Gs;S?rWGTkP'*ZcwH"
		"s4NS#F;ZYsGJ%4JMr21BCX$FsC&j]tj13dtotb^$YV)2emTHDsVvbIs^7]]$w,j]tV-(s)N->u<[<D:Qnbuu:XB[8[[[5csTsvCs.Z3csibGepOoAK&@?A8m>I+)WYHPr-?@'v`@P6s$"
		"$4?I:q>8xtXq/LT@2W1%`o/[t8g1N'L:0#IQ.'PpF+<M,bpbo/Wq)q$9,:D,vP@vYN,i=T0+I6?A^oZ$rfiI,8ZW)ESqWCK4;f^$_>4j)Mh$#uJLki$+w3CNkirw$9n^tY'31N'b#3%g"
		"1]%Zs@LrCsw%M`sg?I&*TcDOsf>R:?N[&Q/I53:?5#]htMsw(ubK7N'u23as]Zi%tjOhODsdjltb%&Y,F14)6SkMe$h(^%mOf-:?&<6MjA_`#5FXiN9N@BPsrj8csX+XRA=-^(uehSF,"
		"e%;sHNfn1-cLwCuYTeh9=._/ZFujIikhqaQa9ho+#9tv1UiA_eqTsKfLj[%k-`?qLlKxS:q:,Fs(?3`t;+f.G;_gcE*i$e$mw[W9d?7aEJH+,-IpFvGSh5l,>4NAb_M27X.RD/ht1=Ue"
		"]?6@%iHYhB:FgJLCtb/-a5A,RbV%GsMnhJLE7s6E#A4(Nw4QbDS:NbhWra6$0<sk/;0)uLD%^4=u5l+%h.3rWftw[=GWR`k_&gfVV%6T@8&SbD=I/N'4(VP,(E6[sAGNJ)BxA41b*M4%"
		"Xgv&J3Z'_KkDti+TALE,Nqd?&ZbJs?G&''%JPjjYLkK%S[wTo+-3*>&>72`tPhSF,jboEr$Ho_6,?WE&rltV$V)un/P[)q$XxkDEE^(mW^F5J`Ak.&:m@47ev%@rU-(<h,m+Z`s32,g)"
		"V]JDsAFOq$K0MUmpA(,%<*$+riK0N'h53l8qJ9SI^EY[s2IVvka6-$tOYJ9&8J,oRQ'C^4=/S`tKa/:%puhe(>?4gQiSVFse'*V$W8hbtYQ@eGWXHL+`kb:&@9n>&2jK+2?4V+rc=SeC"
		"E>2kL<m^[GD<+a<m/hLKKe4T%-[IT#lRX6$%gt$=5(J5u'Qtq6Mq8N'I;O,i9,MZsrx:`sh,ne(#p,1Kf2'4^OAw^[UFT.(UGRqCg9,rH#T/vtWPfnf6v4d$DDbA[Xk&GsRx)-]Ou/Gs"
		"Dve/hH#q0K#9V?%^@Z$ZTxi3lXM#fC+WQfM]kDUs=O5DF?0_I,4P^Rs2amkA>qB>&W;/+NdsPHVR:+^A,`][sgRJaN2i$A&A@>UZJa<1M4-NJu5UA9&PPuVFJcaxsop>=-7FSt8>9UEu"
		"u<37'UnxcL+0]'/nw68$0Z-$Xk=>N%4>k$OACA+MBCx*M1u3T%YxqFs9N.At1o-^tB]9GHaGC--Oq5>T70:@k?sk?k]:a'TG5>;I+uLQW9DjH,VRXI&W^ThKn$k$Ow)dwcT2a@%^*[>M"
		"wx61K,lO]uaXGh1pPB@4U%mh:3Dq0*^.ugLQBOPCp*-WC]2tFs1fi=-p*ST%(=(I,a81Jsa3,7@idZAI.Zm7rHPRws<QqFs#nq$[N6(rtajVP'3?jh'GImd$sF`R%LsjA,+EIsm3vxE`"
		";I`o%`mm8&(0=10mlL%uX23as)<1oCNtdK^Z-T/`Uq_bDUie96$2#wt8l1asVpm@lZgph$j[lI:Y(Ce_36^Wj7_:Js6Mjdt]Ko@WHl8;&PWhI,[D6N'NGIG&b>qIB,4ST%<K_nIGvA$5"
		"ZG9dqD$fEsQWc$X<I8[svgrq6O.Fr6&wQktq=pEsNJtVhKmPv$ARi,)1H4esdu[`48h@_WaDf*`*+^et+WaM,Wp'=(lZs7%wrqS,4l7'*S3@(<Q*A=,QslFs0L+pR#i17URYKiU0']C<"
		"Xj+>%(@+hH%402pTYSbDI_6%FFt+sL5sZC#u.BUQ_ZT/U(5Zg,g44ZK%5I;7gGI^KbS?3%8_%?UU2R4ATRBPsIMrCsa3VP'w]1@'LL@IstJ348$BWIL08IC<gIpnd_P6_kSYF)<:8J#u"
		"5]NE,UlltlY2qL9&r'L,a9JRYU?c5o;9^b*K,CE,&mo$FIqxA&,)@jpg<2=-]fYh1,0DW@&]5asCj0-),g'5M+s6^rcRg`4R_N/.w9P]s+PBv)n=XE&e#HH,51ERIh*pA,2[<X1f*&r$"
		"[w<o7;0ZpLGgpHs@d5%.o*XcMnDZ`ML[3Gspsj^-LN?_8*Y/HV8CpsLtW`oG%r&N,YL#C8I.^U66ON/sJe]D&=.hh0)J;br7f`Veo62/-UGvxY.J`VeZ/E5+_8_$bIu9Q0Sn@D,g,/gL"
		"R`eG&Gb/RnRKpY6x(tVRIu'XM6<e9M*_uoJ>uO[jlJVP'qPfN,H0Um7(msYFdqGQsKUoG31a(*6^Mu`4_oQ>6v?_&=5LuCsON0(*KCE@;hfp<lVpmn.K].ZsLXj$Ge`>+2Ro]?%v0Xct"
		",Kc$G=C'@%Y2^[]H%ws$K;Px_L>5u#]2-K#1soU$2wY9&$kvCbtdR$9mf1BaK8>Cq0xZ$XbpN@&)gMxLktwntMF4E&WN=J<A8`Rp_D-AIM#:+t1nDSJiSYR,6FHHb45#C/IUIntZ<(J%"
		"2[=#GP_w`j^`e%%Y<dMK%p]?KJ(EsHOh^k,*1xcVu5%1->]^v>@fNh'07sFsN76as&5+FsseDDBRH;`tLwIHsYqqfhn`h>VRvX/_3GkC*RS/f1Zd#TFWoCC*r$_:-V_b;JVicP,B#TYe"
		"_ENt$<@QIUkr@.)ZH5c)oQ6K1-lq'<Gq;_tVjbIs<KB*<ZlCC*?ZdtPnO5Og`^'h$k)1LT,=$ghEZGN%-.qHH@7uYddU.Y#e:'OEIs.(WZNtVE=[^X#NK?)r5l_MZodSFM:SaGs'`o9;"
		")7*Q,@2]J&s1LP,qDVft)@/e+s6jRNtQ<=,q09stP1CvY&&+m,aD,:H(&hEU_W<O,B=Cm8@6`'-VLjsfrnwPJca`BJMfC?%M:5`VSo3x`Q`83-=>tE[bnbn+U=C*<I3A:-QvQ$t_n4(4"
		"&ncft>vaLs.tVw$C9#iKD-gKscBwR.HFK?-fi<7.',7&cM6eM1(:?7^oP`1`;Yc9vS%P$<W#Fh/kOS'j>j;g1vnaoT1)fhgDh/Ce-#U]M,7*_n2Pj>%gmbYcahx'jb8K=%vB@:$bM`ls"
		"sLaGIFahZssOm0U[/KI1U3-l$)$&w_1/e-JrZMm,f3ul`?JX,)9k2_@8XWu?%)PrC_ES)?VjdiaSYb:CoAaJ+w.]gXR:Wc>g)Ztau]sJU%R8eJsllB_8ck2-dIln>`^p-L/]po4nV0f3"
		"Tmxtk5I:rl5KJ`,(OYG&2)T(t@[/^t3BsFsBI2DsFah4gCN_Es)eR(tFAX%c-VtFLbYYa5TF:M,50.8$surI,.<7cs>Wi%tHBBihZP;D`fH0A7>lpM,,Qi%tSEoDsQ-g8ntL47I%hlWO"
		"=6.I&tA#E,vxpDsZs8cs3W+C&:@je1f'WL'Ff+4M0$n9SHG3D:5PpL,,6rK,%5];&::fR2XPM`ssmw)<A^_O/w*o6eoa$W%o,8:-]mKkfb%p$R)V%_kxOZ*L$['D&XGlB&-&<:&g.rFs"
		"HU6asu.fm8@I;p&Xn<vd)hd=5/i<HNjO4FD'Ug0MXE4jLd&jB:^L;kFBG:kFCT[;&&<3M,I1Z0G+Zp@&or4G-][H]/.8`c3iG/.(^e;Ia+5)V$S$:wke2X(L)k'D&nIU0G^)<:&L.rFs"
		"&CR`%W'o;&1#%qfV.NZsAd`(jo^o'NQ4.+<GHvNs=ZAVRMkHU[Llk2-0h1)>qYORek0x_<1AoM%=P-'XsmEB,$QfXhpq3ZsVBXxkIA;#j`k3ChIe8:%RHDXDlm./R:5JT+sE'F&WH*@&"
		"a*Rt)g/@*I:JQVsLe^/%Om7*IcVsP$f5[EIdJ#d3oB;Z>FqiP'N$CG,6?5T,,<nvsj4L#ulJ7cs^u1?GS+c[j*MRN'$7b-IjfHZsxD,w#cJ::?V>pF;@F4[s,DL9hHTrq-tUsfOC0*3h"
		"Hr;Hg`<*_j2ex(MSZe]G8cmJs(FOato(KEs7[vciC4>fA/S68%pSK=#`C[HDI'_Mh*x3sL@FmErCMUWneG+16$=>FrF(diMRaxvtbvl#ux7wDjufVP'vONF&WwxZs:`iB5T<Fbsa/[:?"
		",Bqt58&5d$]TvJCbKVm,/[=e?8w2/Mh[Nqm#F$_s4PlYs47Qv$CYhRIr0pSMDDeF%nJ7%OJuGO,dBbr]S2UF]6%+x4wn#l/`8>dM(/:(E^Y]CE8g%T7>=4wsBMo*DOZZeCGWwxtmlH9I"
		"_*Aws.<UMK[gZG&uE*w#6$P01=l$1B^uuPhR?t<Yi=cmUbE(1B_E8ht^<Bx7SHI*j)&K@k9Aj9&Xdc,)H2f7%n$g,)F4=7%k]0@&(q^O00AIF)m--e/oqjhs+5q7%<ba3=&_>Zs4:rDs"
		"+pmp-21L-QSC,/q;7RJ[)(jq6DVg&45^Hs$:2&fConFkfW=j^JkLrn[WANn[-;a&KIRpN0w`)MgU1]?j2fRN'3eb-IJ8wT#^A0U;SLdpscJx8ubC/@FMsLws;9V?>YnT%kBnqC,xdm.M"
		"I)ivsl5^x$Po:f1LVle1[LGDsbmnh$Kf<9[vCoqH2'_@65uL$u:D#bs@p*&%GsqC*ATLC*i^>j0>Xa1%-u_R%/1.S%59+Q'v?En<;)u=*@r<XY[u&=PZb_]tWhYft_T3a$`%L_E&dG4J"
		"ZTtrmf=wKs)jAetLgP^tFQoDs%#O>>5.BQ/d5evs0k'2KBAt=&wv?XP)pt=&C'x(65p@jpBs4h2PR?Fs+CZ=/n^Nvm1(Dr$3cmiLrJq;(gf#7I'O-'X@o)C3[xKEskQ-a-/rqFs29/Ba"
		"=vxJb(FL`sO@8+M;.k]XTI4LZjL6EEdZ5xLrNE1u:o.^tRg:[_9]j=5S5:Esgf;9c;e<T,FI0Lj#@D78nrhG%1u7(Waqxlo/D=Ds$2Ku$&?N]ttY:F;W4M4U%p2L`PZCG,_OOJ,8C/T,"
		"61Q*r.sCp.*k.?GT/^?^Fn2asfXE_&5gm*)1b+Un<E.1(ep*B,`Xgt>+WGs-U16EEk<@Q0p(.@FkxY<-s[<X%js.K,:M+48#MHO,B.PDs1Wlka'R9]$$5DctbU8]?;f'4Zta$@%uqd@&"
		"dCQ#l+5')jS`wsL>;x/t=fSP'oVDJs[=]f;c7`qH/?[G&98q)48'uw4Z6^K1]cTmS+MWi.*7F>H)?9X1)XUgL$<2=-DgOU%[1.48^r[b6w6QT,#HXlS<_bb)v^+dpg?G(D<Us9%Yo^OG"
		"kiJ'h@?<t/`<i4%FcR7V,%3h,QOxxkm*)Fse4TLT]=&jK6'6D*oOKDst8*X$rE9EsSkg4gdTN_jp`M?8JG&M%oP^9vYr3@tqAe?t01?wdKg>D&M0?biA`jc:H^S49pg_;&Mf1`t4xH36"
		"P<Gd2GO/w4BPNF&V.hh0X>.gs6$/@'=Mn*)(XqHid<$u5vNTk-FH6hE`2HT,,JcPj_0@>&j?ok&wMxLMjJ=0MVX[&Mua3Gs`'hhLa)#d$S^b/)e4Yw09vnk8x(kfOF$VXe/7WC&&w#n:"
		"T%BHQdQ=Is`UdICY4+H&_4GPs7q148tTsfcEG$l)t5A?&hrH86@F4[sFP2KUPE&ps6Sow=EV^@4+VUuRO*w$CGl8q%?&oh&Mgw+RMmvmQsQ0EstO31BZp+%#.4>>#`TXc2Pv8G;E(H]F"
		"#`b%OY+IfU%^j4]i1Flf`vhlAM(:DE.=hxOW%RfU0GPi^W#v:drm8/1G,(2T%BL;?@2SJCBS%)W.#05]M*X`ah01a*$v0#G[)KJ_0k`cij,s%t>n]2'r51d2$r%^=5*U8@pblYPXScPS"
		"2#bS[)bIDjVU42p$pxxt81Rs$Q><a*8;V)3o0#T@Y_dAFWJP5S+,G2^hleSe^qXMpp2NDs1fhv#]Lq8.p_n>>NiX,DhK/ZG&vZ2Kt#[;ZS<'aap44pe0X)ghQR'jp*8lYuQDj&+1^5K1"
		"Pk#<6hGOj92_Xv>a$<^F$^h5JHT/WQg@@jT;DBp[`)d>c1bM,iQxV8ne61jp$d]AtNg_g(iL5?,>`n&4mop,;1O+?>G/s2Ba*a#G5x+HMRs8WQe.i2T1d3WZSt[,`iVi;d70S)jV1a8n"
		"hEU/q%aJ&tqwQ^=P:5c0]NDeh%.:?%PNVxCqpvj8e6IlB4OCe3B*j]t-x=jpY:9i'JwXS&_PbA,^]Au,lLX87@_I`t8]JOsX8he1=Ooj0A=_gC_2[r)$u`L'+X8Q&,vOFE?q8OWKfYL0"
		"5*)-C,8xI,$8^b*bX0>tlLu#%d*W%=AU5f(+jhG,Za9NK_G-3%6U`#UB#6mS,D)h,cdb)Sd6jd%D`F$*?@?\?&gPmA4hVKU[KfX2-h?plU`8hnntc_[4cYIJ,4EJ#udsWS&K^_gaFkxPJ"
		"pN:7%wKN/_4U&$SH.Y2]bj3X`)Z;vciw,b,)ZAj9Zp@(-p'&sT;=wg,c2_TL+GBK+a-FLW_Z'Gsp-m>W4;I/)V8FXGWIJ?KMt91TeFD?TtX&P%A9F<V-/7-%LW%EI4_[NS:uKe-Wr=g,"
		"roJe-9:B<Vp,Q,V%B`k,c*bGVtaX,-/-J#uibUQ&J#XS&D3;L,iQtF,^g`LauKUV&Z)t'?d8wqMHXB*%x]-Q&HL.^C)L.9I@,3-)1%/-)vCZ?&jNtw49teh&MF_L%oxacR(KnfqAN?t."
		"Qn9g@Rb2K=Y-/S%^CGg)MLA:F:W:vL@Yf$V]HIW+PdE*%ivw`E'&oG&d*tKps[1^t#/)]J&^;`thF6FD,M*wXihe+-,4`6TJV,Fs;Em@H:3d:&RI1)Ra1+Q'Fe=-Q.,7(%.)hOEKM:7%"
		"xH3j^+ML`QH.Y2]d)96b@1l,=JdvfV#or?%_[ZYlRRjGVv&Y,-]7O;L2s;`tYd'+2$ZPEVvH<:)SPw9)<vF5%'#U29O[uBa0s-T%e4rwG*WLe3Nq-SHMM@`th$m3%.*Rt)_5I5fL.F`,"
		"x86^47-V'-wvU?[`a'Gs.wFmYfP*2-^B;jt$hU77@2o_$EoZ_3jsk(-62,g))'xjOl:V.MQ9CLGH,t&%_VH0Ze/I&X8G=m,2IL8[p<x+-vm[i)JZ>_DMM9`ts:KuASX2*-YBnTsIcw0K"
		"pJjm$h9r[F_Al0-YTq;6cQ_X#W,a1%aglpk)RhOLt4F7DE1vqZMU^C[D3DR%,@%$U6cR%_x:aFssG-rWv'iP'ZZEB,p%=<&IrLQ,mR+9uVjbIs,9qXG<L`R%.3=C3pdSh_x_EZV>lMss"
		"3g#[$kZ*_t.]aX$'X9OJ&rjet9OS.(^NU^$s[+r)3^Hs(r@*UedJ@I298L%uI/(v$-TDwL$JS]&21G5f/D=DsW4`rtC89aso4+Fs)HvSFI';`tlZrGsqj6,i4%wK%OQDACqbu3%hTM=B"
		"mDM<%SZo<^ixattY^_1f9,5d$Fl9'[UX&GsvChJ[XaoS&Wv7]$V=$'TH2w1h,<S@%B<>-Q;KAst,<`B7&UuiZA]/Gs#ND5GJ-;`tj[GFs,-7,i0iZK%PdrxC>BWw$os7uBgvl;%RHA[]"
		",DbbtTsU5eEi[S&QL0_@vqNe`n;Ch$3$$c%ukbIskYdBTbKPR+l12lGMT;`tY'4+Hm[xl9u:Ud$,vMD>A`OitZIt;Lm2#O+2,kI%i(anXghU,D4)*piR;MQhMo,(%^raATm8RwbRmObD"
		");3+3moT%k%v`qQDMcLZ=F'XuPMw_<3H.9IZHOC)g'0-)K-hF&dNtw4o2[^46wPJ&JdKJ&8oE[XjG9as4Gwt)O@w_<suId;1M)d$wJi*M*CZ]tQ.6FM`X,N%QI%BKgBP2KjY:--3,M^S"
		"%w]udG,?aS6<U&t[kWS&v:QD,AVWP&'ht4T7?99WKm@vY%XMc,qo+UiPDpTitlCXs(%sfQQV$s@b/*RN:sWg;-[/Q&X^WO9JgFA&]MP%ur'4csDdL%ug,?rs:ua9%OTMACT)YMZ$OnAX"
		"C@plU-FJRnCS-Rnr@Q'Ve9VM,cwmM^[8Oe,Ev[9C`R9/;ir)fO?b-Qgl28h&Ja^S,t$mS&Y:.qdhse=&KWmV&[x$t$;<*,]=<OB,6ZOB,'n)Jh),+DsY;5g)vAnd)M.?Z](0f%c3A^8X"
		"roRxI-wE6J)>6S%>=cls?4xDsq4Jst<I0.9]XKutX'=Ls@Id*jsUo;6smq_E/N;OsF%+h:[+m<I6,Q2T/w;v_tW+dRC3fWqn4fTMHG<`t%<Y,;W6bA%TtQfYFpj&%7O?3Z:fH*js>isH"
		"EI7^&'lvi7f=f>%9/]FD7an'Ei4QP%h0++D6l2k(e?p<dHK@I2N/Xb$U5UA&RkuDsP9qj%Jtk2-FLpqIc;cg,]R048l'/jT&9l,-,.o#>U7#[Lx4rC&d8AT[LfX2-O.)P,6QQ#u1RZS&"
		">%$1hg;WDsEir[$^&k<8?\?n]tK%&]$>?9ase=CV%MNWDs4-BY$m4H#8HZn]t99BY$AHT&thkbTuNHvC[Us'fsfvltlhHplo^W_.9&9+oLEOI5)4Txnj'O?K&TD)pFY`cS&PVWP&LiSE&"
		"5N6[s'v[&tg(/%bkJ0qs)_aDufw+@bjcG3ut7jmf[D#AkgTH_sqi=I%^1FL9tj<`t#PnZVb2tgj?:7^&awbeNF]e#ghC4J,Pbo5ftIHUH97)R<vaVK,MLHR&OS$_&5oVbDM$V5/54fS,"
		"^$;mLeFxT&9G1`ttQvZXo4-10-SF9%Tu8wTx:b$OGn/xTcHJ%-[weUJ?dM@Od[[l)X97aHbo3-)v5`-IU5eJ,^8mGV$/6g,lY8%U1=mS&'6Itu.]MZsq,PTIF<GT,rPCD)vLbCN:#/**"
		"t-m]tnwtV?T/Ug$0l7N'X/rbV1aMQAGqn%u>tAPs@)`C&EukkL-o$FsEO_V%0<gwFP;rx$3;nvsU@DWZZQ?B=/Dn9vlt+w#)hNbjc+=HM/B]NIuFNw$oDbi$-eV#lcNc&4-d4f(F#$c?"
		"\?YF7$s[nR&(5xt,wYQ^t*7Lk$,<3NgK$x>I'nHL,gQT$f_2gSmtvni&.Y(d$);X&m1XoYs[5h?u@*'19SYWWSA*p'-J+R#lJq6asZGrP&.a5I`BacTVY^eS&f,xw4IpBZs7&`P&JE'(3"
		"DF[;Ha;(Q&C1ws$(+31'Y$%K#A1rP&/KYS&vjInLY;l)mKox7$iuKLhARj_P*[GDI?V4f&+)419iXFIsY&nd)kB5`*p*4N'LX]ZKWtJZP`9dg,dp4B&X#p8mCu9%I=Q*K#;&=^t(5-.1"
		"fQudh/^3%gihWZs]-fpR7N6^tgQAqD[-;%E67^e&YE`_3Atd2hIKrCs^k1Ds#dRKsp1u$F,8=K1RV^S&I;^S&?VSkj;BXxk<()Um4D###?vr.C8F#&+ZK8D<n985&s1_V$r%D_&*UJ)X"
		"<l(m/KQ$4Cap4A=D+##,pUtx+(Vq(<7qY,EcKl+MW/>)4ZJ:&5j'YT8JWh+MNX?J:UIDxH7]###Ds0A=<Du(<F/rx4op2GD0L3F%^oJV6o3w(<ThOY5.t[p&aPor-F8SN:,OVf:1cef("
		"&i0^#?cR^0h2=*$cfUv#JY<rL?9(sLUh?lL$A2pL6X+rL-x.qLv(doL/4fQMEShlLifI2N^phi#1uqv#^I%d#xF:;$sw^4.m;:pL>[>e#j==e#h+*q-RMYQs7+Lk+-''2#eR/(Mq=0vL"
		"2tVxL;wXrL)``pL[9.*N=j@iLbW&%#:W]vL2iC*#_DB6v0t=6#B^h3vuhr0vm?J5#T)MP-N+W#MfZWmL4jYI#f4>o.::b]#=-ex-J`Kx7ImVG<A^jD#iYe[#E&pa#Hic+#d9p*#n+Yu#"
		"Gnl+#n/`($qH+kLl+3:/$8A5#0(G%NLJ_1M<A;'#q&W<#IR7%#`ML_&vGVwK$9^c;S<GwK&eW]F?,fQst;cQsg%B9r-&I2CHueQ-06lwMWt11Npd#<-D/n6%,>T9V#h/.$<,-&MT76DM"
		"1j30Qs2*6N>jNj0I1ml/9*',2p#NcDgttP9.l/wp)`XxX5,JS7YM9-v6U=G2hO*;?*Y$s$hHT;73K9N(A1gFrl.Ca=aY%)*1umD4-fDk=(w/.$T]GLM2oZb#Q/qB#SRrt-kpYlLeu#u#"
		"L'niLq(`#$aZ9hNO^mP.Oo@S@[vrB#q@uu#c.>>##@;mL8-UE#%M#<-c=Ru1bk*]#RS1Z#coM<#IcHiLg5VB#X'niLnoC^#6Km-.4G6IMSL'=#fK8r/VtM<#F[<1#f;J'.EO&kN4O[Q/"
		"&vQ]4$i?xkoo.Dtm]Mcs;tjV@mqb59ci*)NCA/crp1_c;3mq^o'kj-$/-k-$4<k-$=Xn-$[]l-$`il-$=#HF%i5i*M.NZY#9+uoLv(doL[6wlLHotjL'D;^#`XI)$VW.H#lm;%$:K7)$"
		"5->)3kcv_#`C&&$CaId#jZu'$@<:pL_r-A-MW_@-E7T;-u5T;-$6T;-e5T;-p5T;-(6T;-r5T;-$`&g1TL31#Fn9'#lxt.#_MwqLMME,#KFBsLs^M/#&Ui,#'?8vLB3urL,*jxL_:B-#"
		"Qq?0#lY3rLlhpvLimsmL;vRiLen#wLU6'5#KK.+M2ecgLROEmL2'2hL]>6)M,s+$MtsVxLV'C4NEE:sL8LoqL[FN$#m]#3#So<<8VTP-3>am`*@`gS.p$#,MAppV-sMj--.3k9;V=b.$"
		"hh#9/SPGxkA-grePPN/i6]>>#R*P8MQj5s-@<`*M#f=rL5CsjO[sZQNe/I3#k7u^]V_P+#6),##-`($#d$Y6#gwCEHDTl##4X@U.#)>>#1r:T.&/5##3X@U..GY##=q.>-7M3X%lP;<B"
		"H?bl8:FmuBFr9eG8#R`<m<v<B[w6fGR[CS@:c;2F'M*8MUAg*HeHx.#S=XS2<E',H'->/#^J:L,L=Oe?vLO&#s(B;-t4T;-*lP<-<fi6M-LYGM1e(HM*_uGM$:>GM)XlGM(RcGMZ-*$#"
		"Ike%#GdsjLEd4)#a[P+#1h7-#Qst.#>u5`-=*OF%)md2NpLJ'Ne_R`EZ(5DkLDS'#oZY.q=?UG)Mc#(/PPY-NXn).NVbm-N]0N.N-lR2Nf/(eRi,'5Sb2JM9COpr6Zvei9PjBrmhpmCs"
		"l2N%tpJ/]ttcf=ux%Guu<bsl&@$TM'D<5/(HTlf(LmLG)P/.)*TGe`*X`EA+]x&#,a:^Y,eR>;-ikur-m-VS.#wNM0)EKJ1A7tu5Nj^7n8lqi0Jj>ontZnl/GUTV6S5Z4o&H,,2%KGG2"
		"/pcc27D``3:fwx4@r[]4Q6mo7.)vWU:tLV63X>WN]LrKG%PlKP,(r-Qh$S-Hf[;kFl*/LGn*sKGq?S-H?neERq$687*MN88s-m9V/Gvl/2Cjl&mZq.C1435&UoSY,jdGrdMbGs..f2(&"
		"<+&(&^;>_/D_5F%iZ?X1vD<A+RT02'aQDp/hS>F%VFow'5qLcMorM7#O/5##<q.>-Z^r.M>0F7/3'Y6#[RtGMo'<$#>bYs-vaK%M-lrpLw[H>#u3G>#'%###u4T;-9:)=-j5)=-0s@U."
		"ic>N#YA;=-prrt-@KwqL[>WmN[IQ>#&d&Z.D:r$#LTT40C8J5#C+1/#h.NX('POgLCkFrL7P4<#UmD<#Y#W<#^/j<#b;&=#v.Qt1V;P>#,Gc>#0Su>#4`1?#Q2(@-GE_w-3=id*4wj68"
		"6W6X1IH<3t2)S&#.U]F-*6)=-0%Uo-V1c'&V@K&#RGJ#$i0$B#VK3L#Ni68%weT#$ZW3L#T=NP&#x5Z$l54L#t7-5/CNm3+xGP`<-k[[Huqdi0SJPlSMt](WP;Z`3L/9L#4gf4]f7W]4"
		"g(:L#BRS%b_+3?6t3;ciR-<kF)qj-$I)###UH3L#XV,eQOGYY#YT3L#R135&xnp>$^a3L#NEcu>1%Pk+a*8SRB'q=Y](8>5^c9L#?7W(aX]Q>6sTH#$u9-T/-Vd'&Mg?S@^^dA#^RK&#"
		".&1#?21XMCF:pV.pT(@0A+*s@4I_>$gRm;-h9I:Mk<$##Gl[+ME)^fLCp3<#=)c?-3(m<-ldd'8UoK#?Y2i+Mo(^fLp)8qL*#kr$J;NY5gCMn8wN?_#0x%/GWeXUCn%GG)_.Ph>0S^v$"
		"`Cf+M[xSfLok6##&5>##Q9Z8-#h98.@1xfLT'*$#7*:*#w>Ur%E,tr6bu/AO3U=Yc.^Prmg)s@tA?uf(ei,)<6I:AOg^drZ8V3Mp?p`A+c=eY>(h35S[lAMg):xM'@F5s6W>3)NMAJMg"
		"'A:Z#:YeA4=GB)EPfvfUd.TMgwFZs$4`/Z5OX%ZGVGP5So-B)WIBlYc`ag)3kU3Z>8*AsQW/7sdkMkYu(ah)3;)FgCNG$NTbfW5fu(_Z#2A3B4I.)BFi3uAX.E,ZlwYfAt#6WgC_Rf)W"
		"jfRsde;0ZuhBSN0kJON9(,ENKT)gsQ>8Vsm<xfZ5`KtsH-v+6]PI9NptgfN0A;tgCee+*W*F`ghoA4Btn<W6//*M6Ax>1tHo^dZYDJrsmGp`**JeCt6n8Q6J;c_N^NPdZl)sFBtbca**"
		"Xj&O9OMxNB%E8tZH9=8%X1'crNE]=l]cM1pj'?%tP_.F%n]0ci&JD_&&>ho.#dq%4xuJe-?iGe-W[He-I.b(WC;>f_+Y$G`8ubY,Gjx?0cGB8.BG>;$5cNcswVC2q.h4onu2KWnR:ho."
		"oIw%43>+XCr[Zf::38G;NPxf;pkM,WFHAf_6VTJ`JHvE7N(D>#<MsOfF;8>,M#BkFaT,@'`(Mq;bJYc;?rK/`cGbY#?=0dsfK#Vd#=+_Jpo2_Jwv&v#F8RV[Z6*dapAb@kFB8p&oEC>Q"
		"&fF2:n5P&5q1e?K$],XC4kOc2BOq%lcc6&YQvxjk;9R1geH#VdG-?JiH8QkO)`kgL=@UkLLS(:#CA=gLM+t4#h,p7.u1]vNKZ/%#%%kB-udEB-gAqS--_W#.hnRfLd,aaM-S448e)[;%"
		"_@xcs)3dIqDrA>Q1qm%l@5icNaV$&YH(B>QcXps.>du%48v]'/7j&W%_v7j_C$&20WdUJDQWhJD]8c'&Cvu92wA-@'=8?8%L2#EG?,fiLbNcxum#Su-X8*jLt`(#vAAB>-fdpC-<HII-"
		";4RA-LbN>.LWt&#-nx1q?@WW&9qWK-qkg%%JAFS7*jt(<b`d(W'U+kbs3IQ(j$Y3.*bCaMVYuxuB[V58k9S5'Oa10178p*#*C,+#/I5+#dee+%iEAf_:cTJ`ZSOw93^]V-(8mEIP@+KE"
		"X*vG-(<D].NeZ(#D[AN-eo,D--n,D-wm,D-465O-=tu5&Pu@kbTKHXLdZs%caF*d*D3=,WcaB(JrTaY#qQel/XI,2_9[oT8t3YjDYK_+3Z<5'v%HG'vSNP'vw;:3vT,O$MNpdw8sL2<D"
		"j>8>,7,U)4P*#q9R^PWO#-L19Hn<n4eBD,MY>m6#LB58f:OEr8f=794hDxu,jfHi$Mb*S88.Y8p;dmuR*)95+Mnw@#XuIO,og]13v95'vfGG'voMP'va5OxuB,niLtrlxuuJX&#WQ'#v"
		"r$ffLRMu$Mk3O'M?)F,:UnaDu6igOUW$aC,I%a1:7k3A';,FS7mv,Eulpa$#4in@#NaqR#cBbA#T7%@#T$b30__R%#YcZ(#Q6>%1M)hw0mJg;-m0'C-%U<nL&L6'vP_ST0-wiEPUCcp7"
		"sbf-+R7]x7<&_E#[8tW#hv3PUKx6j)Udli:eHG96IDMV.OWt&#+v<Z%.WpI_N8X1^dE#VdUfpI_GG]Y,ZiDX(wYDX(2VLJ:,KHJrt`2eddS=%tWN;'mdVZ)M3>ABN/p)*Mlp3<#$Ti]."
		"icZ(#NK+Z-aDc'&QDa3;hmJwux=F&#%0wiLncc&#^.xfLocbjLL'2hLu-<$#oZbgL?p)*M0vu&#qfv$vnQ;e.;AC3vWl*J-i<8F-q<8F-/%jE-_0/b-<U[w0r)c?TLML'S<JF-Z`nG#$"
		"9oNcsqZ2@'v7ho.w1W'AKKQ'AC3Q'AmONq2;w/9BVZn*.d:4gLk't>NP'2hLNwfN-_4f/M48A>-%Y$m%*hQ1gJaJp&hNfQaRa#Ra]ru;-%'G?-8fF?-r/*q-VJPq;SgwV%n.Cp7C#CpT"
		"A_rr7W>RW&%QE@YYbm##9xr>-eUpw%wc(:.oeZ(#4umr7Y/X2i0r/E#fq;;$;QZ`**^)SewYg`<U)[;6U](#l)Fs/1&1,$5%%x#>4u=HMitLT[,B10UxDHTnqgD0CILZY#^9ku>xigY?"
		"#s,v?$&H;@.irs@o`[iBuxw.C.+XMC;XtiC4I9/D5RTJD6[pfD8nPGE=E.&GEPuoJNr:5KGcUPKHlqlKIu62LJ(RMLK1niLL:3/MMCNJMNLjfMOU/,NT-c`O,g)&PZKCAPWH_]PXQ$#Q"
		"YZ?>Qs<=AYtEX]Y.vnP^g1D5&d(%b4RLdl/@80VQQb.F%B*0F%?_n-$&jp-$'mp-$Cw#:)C.3F%XS5VQp4Xo7.:g=Ybvj-$,$k-$-'k-$.*k-$/-k-$00k-$13k-$39k-$4<k-$5?k-$"
		"6Bk-$7Ek-$8Hk-$9Kk-$:Nk-$*dOw9YG_1TmIh9;Dmk-$Epk-$Fsk-$Gvk-$H#l-$kJ'F.K,l-$L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$UJl-$VMl-$WPl-$XSl-$YVl-$"
		"ZYl-$[]l-$hoJwBo.KwB7Bi=Y/kV?^q%_?^mo^?^I(]M'$4xi'.*O2(/3kM(0<0j(1EK/)3W,g)4aG,*5jcG*6s(d*7&D)+8/`D+98%a+:A@&,GUYs.?f>5/EOlS/FX1p/GbL50HkhP0"
		"J'I21d#fM1L9*j1MBE/2NKaJ2OT&g2P^A,3Qg]G3Rpxc3T,YD4U5u`4V>:&5WGUA5XPq]5YY6#6ZcQ>6[lmY6pW-n9&Jt+DcTe7R_>c'&oX=rdc>,F%iab%=4D_1T&5-F%08qI_/8l-$"
		"<$EcD`YNk+o@m-$L9p92x[m-$#`m-$$cm-$_OvL^74Co[c'n-$:B_w'3R/F%4U/F%5X/F%6[/F%8b/F%(2ArdnHn-$A?G_&1<k9;?_n-$^IPk+Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$"
		"L0o-$M3o-$N6o-$O9o-$3uJk4Yp0F%VNo-$WQo-$XTo-$YWo-$sMp-$tPp-$#VK-Zw<Fk='mp-$67bw'=slQa,&q-$M-X-H?x2F%XSk--=Yq-$[]k--0?JcDlme7ROiFV-ifC_&+wj-$"
		",$k-$-'k-$.*k-$/-k-$00k-$13k-$39k-$4<k-$5?k-$6Bk-$7Ek-$8Hk-$9Kk-$:Nk-$.W8_8Dmk-$Epk-$Fsk-$Gvk-$H#l-$cWMk+K,l-$L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$"
		"RAl-$TGl-$UJl-$VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$dkG-Z)BM3XL+A2'._XJ(7&D)+98%a+Sh+3)XlgX#r3S>-V5T;-W5T;-X5T;-Y5T;-m->)3YTS=#UDluujX1vuQP'#vRv7nL"
		"=RxqLk$v*v*[<rL/mx#M<s+$M2Ac$Mn=S4vn62h7#3&U0jiLvutJtxux?<jL%`I$vo:^kL,44%vAJxnLR3urLC9(sL%HA#M>NJ#M+TS#M<(i%ME.r%M?:.&Mxn45v%nLvutS>d0`$`vu"
		"e*ivu9q'hL.'2hL/-;hL<3DhL59MhL3E`hL4KihL5QrhL6W%iL7^.iLPd7iL=j@iL>pIiLlAGxuWJEjL3WOjLE]XjLFcbjLGikjLHotjLJ%1kLl+:kLm1CkLM7LkLN=UkLOC_kLPIhkL"
		"QOqkLRU$lLTb6lLUh?lL%oHlLWtQlLX$[lLY*elLZ0nlL[6wlL]<*mLmt&nL>xu&##TS=#pG5s-sL<48tMN*Fw_/]XJY4;-O0qOfg*b20rO'#vMV0#vcc'w.L*t4#]<Kp8l2SDX2d1R3"
		"#`m-$$cm-$TJf:Z<bvL^c'n-$:B_w'3R/F%4U/F%5X/F%6[/F%8b/F%1`tOfKWS+`rm/F%Ed_w'fxj=Yu^n-$bnh--Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$L0o-$M3o-$N6o-$O9o-$"
		"3uJk4Yp0F%VNo-$WQo-$XTo-$YWo-$sMp-$tPp-$'%dE[w<Fk='mp-$67bw'AA.kb,&q-$M-X-HAgT9`CFJ_&]x,F.=Yq-$`+-F.o,n=Y*x/]X*qa%=gO2W-`XVQ1rK`J#-j)P0^r*xu"
		"Rx3xu7iHiLlAGxu;C<jLGPFjL7WOjLE]XjLFcbjLGikjLHotjLJ%1kL>*Lu.H-$#5*lVA5XPq]5YY6#6ZcQ>6[lmY6]u2v6o+J59wbbm9C,<MB(lgWhgt+/(+N/VQ]XI%b&5-F%PY,ci"
		"/8l-$<l3'o0mNk+@x(F.x[m-$#`m-$$cm-$:s7R*)rP.hc'n-$:B_w'3R/F%4U/F%5X/F%6[/F%8b/F%9Ln-$cKA_/?_n-$UVw9)Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$L0o-$M3o-$"
		"N6o-$O9o-$3uJk4Yp0F%VNo-$WQo-$XTo-$YWo-$sMp-$tPp-$m>Z9Vw<Fk='mp-$67bw'1[%_],&q-$-]4kX?x2F%XSk--=Yq-$[]k--+vFo[R6qOfR/LoIN[xUdK:a=cgG,F%-'k-$"
		".*k-$;D[w'4T,F%13k-$39k-$4<k-$5?k-$6Bk-$OtLk+<m,F%=p,F%>s,F%.W8_8Dmk-$Epk-$Fsk-$Gvk-$H#l-$g&f--h)f--L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$"
		"$;?_/VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$^d$Vd0W]Y#cf>8%E`GT&>Gtr-_5wV.Rd/K2=Iac2YMCa4Cf:D<1asc<TxM3Vw/@b:'vbYH*1)vHDx&eXnHA#MrOJ#MxTS#M*go#MEIl$M"
		"2*i%MM.r%M?:.&M.%55vThCvuhnLvugi)'%d>cWA$xOT#WY6Y-D$[WAv0PT#oFKU#.cEB-D5T;-E5T;-F5T;-G5T;-H5T;-%sW<9Afvof`bVw0WPl-$XSl-$YVl-$ZYl-$[]l-$nI-ci"
		"07NP&&eN`<(7?\?$$`_@kaxp-$,&q-$;Sq-$V%78%`&&.-rb.F%NwJpAa[rCalhBW.r[,29QED#$_sX>d;&%VdPBF>dAT&^d>8X<#<8q*8J$(2qOqnF%oru8.H0`$#jV5<-NS2M/@fB#v"
		"oUWjLFcbjLGikjLHotjLi1o(#Ve$nRo)doL65voL#;)pL$A2pL&MDpLX=p)v[o-qL.(8qL7.AqL44JqL5:SqL6@]qL8LoqL=kFrLWmnI-UA;=-F6T;-G6T;-H6T;-I6T;-J6T;-K6T;-"
		"L6T;-M6T;-N6T;-O6T;-3q-A-YN#<-V6T;-W6T;-X6T;-Y6T;-s6T;-00GDNRio#Mc*_k.v2/X#E5`P-bE88RwebjLUZ(:#o#a<#lYlS.S>0:#x5T;-#6T;-(ZlS.TvW9#%cpC--6T;-"
		":)m<-3N#<-4N#<-5N#<-6N#<-8N#<-96T;-MgG<-F6T;-G6T;-H6T;-I6T;-J6T;-K6T;-L6T;-M6T;-N6T;-O6T;-+@:@-YN#<-V6T;-W6T;-X6T;-Y6T;-s6T;-xZlS._HKU#(5T;-"
		";5T;-o_e6/WHtxuB@<jL%`I$v%;^kL%HA#MD(i%MhIf4v(x*(2+/FxuAZ0#v>a9#vbONjLE]XjLFcbjLGikjLHotjL]<*mL_K_&vq:P>#X7%@#Pr0hLw,-D-P#H`-:hNl=#+/R<KN0R<"
		"#+/R<*bWP/Zmsl/GbL50HkhP0hwk59hIRM'I]toSI]toSJwdpS/7e1T0:e1TE>5L5VU@N58%F_JR3-L5wKd%O>jDxLZhDxLZhDxLk)u;NF]]#MK[]#MK[]#M%4QQ#WnXQ#V`/,M`dCH-"
		"-X3B-haCH-%(@A-%(@A-%(@A-w3@A-s'@A-s'@A-s'@A-s'@A-t0[]-TB0L5tfd`a'/###6QxAFS,DG))_?o[JUOfLa)2hL83.m/K<bxu]M]9vi3f]-nZD.-OdsX(s`;onf'K1p2@Y1g"
		"aO=r)v+lX(,Ykxu=5(@-N#=R/hNwI_$`xOfDk5/$cg1*MbrV&NMddIqPi[+M<O5-MLP;V?DP>p&6BO2(LP9e?@ZEL,pVFX(F&>uuaG51N?K51NU6OcNoM=%t9d*SaFD9U<j6]2L'xd1T"
		"'#_>?OSPe$g3r9DDu@1v</OR/p+R(W%9i:ZBF0.$(dexuC7`9M[5WR*=C)W%2oTV-+9<K3hVG=Q<:1UT)'l4SVgGA'@M)&YLq12_O6*eMjI>p7H5.a/+03j_Wgg=c5X&32F:S(WaJSh$"
		"#6;KU*sP+`LLlxu$$jX-v#qx'.lkxu%+*KUaI.`aHLlxuWFEwYdJ3v%T9mxu6qo4QgSp@PSH(q)Mhl&#5.`$#ewiJCP0pI*EXrr$a@pu,Q%lr-c5LM'SAP`<]LZ[H.NQY5j$'DWm1g4]"
		"rNHP8du9L#LesCakB$3:t3;ciR:KwBY%C#CVVuu#8ONP&Qu_3=r'88%Le@)4`Su`414.q&)OFV?Ox%8@SZw6049$5A[wTiBb=)/1f$7L#G*/DEq9x(3u@f(N=j[xOStZf:KgNH<$6r:Z"
		"&YKo[I#gA>;V*XLqPMucSA.L5gI5H2@mC?#TChJ#OU@%#gx*A#h$eK#_04&#pFbA#,=3L#9gZ(#KKjD#LT,N#XxJ*#j+Yg.X'+&#P5J'.u+vlLxVL*#a7p*#hI5+#5gb.#86U@/C8u6#"
		"tv0hL_0,A#+%kB-(X?T-H^@T--kv5.j]>lL%:+)#T>hH1LPj)#t[&*#$5i$#f;#s-Yt7nL]dA%#xeJG24UG+#6`R%#)=M,#CH`,#e^#d.;n@-#$wa?3^FX&#Xst.#q:L/#.@$(#nwiuL"
		">p<4#cvJ*#OjUxLOcp5#nxLg%5xwCa=#MucA%nx=t3;ci05?5&@4PfCA'TM'M*1)*NqLcD`1B>,]P=8.ej.DE:S%&4+arr6,M$;HYmIM9%g*^#drjA#L;S#$u=#s-[.xfL$L>gL=H5gL"
		"ggm##hiTK-.Y5<-OArP-)5T;-,p#-.1?<jL.3DhL8k?lLeC+)#_/5##V)B;-l]oN/c;G###82mLII<mLtTGgL0'(*#mb/*#sF`t-0]imL97U*#jSGs-8u7nLE[$+#xYu##n;#s-UHxnL"
		"W33$#)=M,#j51pLYQ&%#%<#s-nACpL]dA%#'<#s-pYhpL`pJ%#p#@qL)Y#.#IG.%#1<#s-'5[qLYQ&%#3<#s-*AnqLZW/%#5<#s--M*rLb,p%#8<#s-0`ErL0?VhL,<(sLqY`/#Hox/."
		"A;9sLmfr/#23H-.EGKsLVr.0#(@0,.S:dtL1-u1#^TGs-@9cwLbbE4#3;xu-OjUxLdsp5#<<r$#bJR#MAt56#>m@u-gVe#Mu)H6#J`98/KJ:7#*<k$MRw)$#8%q%M:S]8#h.:w-?76&M"
		"6`o8#K$)t-DIQ&Mwq49#iZU).Qt;'MwWp0i[Yuo%9L<5&CtWP&;[jl&A'TM'Gm-)*H9KM'`1B>,V=:8.Ws,/(n0VS.ZUqo.ID'F.[S%&4#%Fv6WJ/L,#[il8*eY59_`/L,*wIM9=<xjk"
		"crl-$XnFJ(2P9Z$qJ*XCHL+_JU.RS%^F$T&&8q;-aQFv-A93jLAdl&#PF@6//K6(#5]>lLJLH##h;#s-ZBonLG:-##o;#s-WsboLYKj$#Z61pLNmj,#5g1$#(NYS.D#S-#jRQ5.qs6qL"
		"nFp-#9#)t-u)IqLdR,.#;#)t-#6[qLjk>.#Ba($#4<#s-$GwqLOkv##6<#s-$Y<rLOkv##A/RM0U:L/#H<G##S:dtLU&l1#9E3#.nwiuLB7[3#%h=(.OjUxLWap5#w&a..vSj,8%T9N("
		">xc._F?<,a<aN9ip$xCaq76&cE0t92*0QucRHF>dI<t922ai7e$6j;63lC?#?=.@#:i1$#7xU?#7V(?#>uC$#?.i?#8x(t-t$:hLdT8@#;x(t-)O$iLe)#A#GF`t-0[6iLm55A#Cx(t-"
		"4hHiL%gGA#O+i?#mTk&#Rd9B#?[,%.Ts/kL;M.C#o9xu-iY5lL-44D#lF`t-#5)mLhK_E#r]1?#^(T*#1W$o@[q6=#";


	//================================================
	// Thanks IUF :)

	void StyleTabColors(ImVec4* colors)
	{		
		ImGui::TabLabelStyle &tabStyle = ImGui::TabLabelStyle::Get();
		tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ImGui::ColorConvertFloat4ToU32(colors[ImGuiCol_HeaderHovered]);
		tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelActive] = ImGui::ColorConvertFloat4ToU32(colors[ImGuiCol_HeaderActive]);
		tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ImGui::ColorConvertFloat4ToU32(colors[ImGuiCol_Button]);
		tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelSelectedHovered] = ImGui::ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
		tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelSelectedActive] = ImGui::ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	}

	void StyleColorsRed()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.98f, 0.26f, 0.26f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.88f, 0.24f, 0.24f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.98f, 0.26f, 0.26f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);


	}

	void StyleColorsOrange()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.98f, 0.49f, 0.26f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.88f, 0.44f, 0.24f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.49f, 0.26f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.98f, 0.49f, 0.26f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.98f, 0.49f, 0.26f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.49f, 0.26f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.49f, 0.26f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.50f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.45f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.50f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.49f, 0.26f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.50f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);
	}

	void StyleColorsYellow()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.98f, 0.98f, 0.26f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.88f, 0.88f, 0.24f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.98f, 0.26f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.98f, 0.98f, 0.26f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.98f, 0.98f, 0.26f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.98f, 0.26f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.98f, 0.26f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.90f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.98f, 0.26f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);
	}

	void StyleColorsGreen()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.26f, 0.98f, 0.26f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.88f, 0.24f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.98f, 0.26f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.26f, 0.98f, 0.26f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.98f, 0.26f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.98f, 0.26f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.98f, 0.26f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 1.00f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.90f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.98f, 0.26f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);
	}

	void StyleColorsBlue()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.26f, 0.49f, 0.98f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.44f, 0.88f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.49f, 0.98f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.26f, 0.49f, 0.98f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.49f, 0.98f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.49f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.49f, 0.98f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.50f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.45f, 0.90f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.50f, 1.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.49f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.50f, 1.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);
	}

	void StyleColorsPurple()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.49f, 0.26f, 0.98f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.24f, 0.88f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.26f, 0.98f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.26f, 0.98f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.49f, 0.26f, 0.98f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.49f, 0.26f, 0.98f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.26f, 0.98f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.26f, 0.98f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.26f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.26f, 0.98f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.49f, 0.26f, 0.98f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.49f, 0.26f, 0.98f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.49f, 0.26f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.26f, 0.98f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.26f, 0.98f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.50f, 0.35f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.45f, 0.00f, 0.90f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.50f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.49f, 0.26f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.00f, 1.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.49f, 0.26f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);
	}

	void StyleColorsAesir()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		static bool onlyOnce = false;
		if (!onlyOnce)
		{
			std::string sFont;

#define SIMPLE_BUFFER_SIZE 50

			char* buffer = new char[SIMPLE_BUFFER_SIZE];
			if (buffer != nullptr)
			{
				if (GetSystemWindowsDirectory(buffer, SIMPLE_BUFFER_SIZE))
				{
					sFont = buffer;
					sFont += "\\Fonts\\tahoma.ttf";
				}

				delete[] buffer;
			}

			ImGuiIO& io = ImGui::GetIO();

#define IMGUI_FONT_SIZE_ 14.0f

			if (!sFont.empty())
			{
				io.Fonts->AddFontFromFileTTF(sFont.c_str(), IMGUI_FONT_SIZE_);
				sFont.clear();
			}

			ImFontConfig font_cfg = ImFontConfig();
			font_cfg.OversampleH = font_cfg.OversampleV = 1;
			font_cfg.PixelSnapH = true;
			if (font_cfg.Name[0] == '\0') strcpy_s(font_cfg.Name, "Bank Gothic Pro Light.otf, 14px");
			if (font_cfg.SizePixels <= 0.0f) font_cfg.SizePixels = IMGUI_FONT_SIZE_;
			io.Fonts->AddFontFromMemoryCompressedBase85TTF(bank_gothic_pro_light_compressed_data_base85, font_cfg.SizePixels, &font_cfg, 0)->DisplayOffset.y = -1.0f;

			style.FrameBorderSize = .1f;
			style.WindowPadding = ImVec2(8, 8);
			style.WindowRounding = 0.0f;
			style.FramePadding = ImVec2(4, 3);
			style.FrameRounding = 0.0f;
			style.ItemSpacing = ImVec2(4, 4);
			style.ItemInnerSpacing = ImVec2(4, 0);
			style.IndentSpacing = 22.0f;
			style.ScrollbarSize = 16.0f;
			style.ScrollbarRounding = 0.0f;
			style.GrabMinSize = 10.0f;
			style.GrabRounding = 0.0f;
			style.WindowTitleAlign = ImVec2(0.50f, 0.50f);
			style.ButtonTextAlign = ImVec2(0.50f, 0.50f);

			ImGui::TabLabelStyle& tabStyle = ImGui::TabLabelStyle::Get();
			tabStyle.borderWidth = 14.5;
			tabStyle.rounding = 0;
			tabStyle.fillColorGradientDeltaIn0_05 = 0.5f;
			tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelText] = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1.0f));
			tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelSelectedText] = ImGui::ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.0f));

			onlyOnce = true;
		}

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.26f, 0.98f, 0.98f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.88f, 0.88f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.98f, 0.98f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.98f, 0.98f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.26f, 0.98f, 0.98f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.26f, 0.98f, 0.98f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.98f, 0.98f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.98f, 0.98f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.98f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.98f, 0.98f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.98f, 0.98f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.98f, 0.98f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.98f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.98f, 0.98f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.98f, 0.98f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.98f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 1.00f, 1.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.98f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);

	}

	void StyleColorsNeoGenesys()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.98f, 0.26f, 0.98f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.88f, 0.24f, 0.88f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.98f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.98f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.26f, 0.98f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.98f, 0.26f, 0.98f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.98f, 0.26f, 0.98f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.98f, 0.26f, 0.98f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.26f, 0.98f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.98f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.98f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.98f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.98f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.35f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.00f, 0.90f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.00f, 1.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);

	}

	void StyleColorsMaya()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.49f, 0.49f, 0.49f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.44f, 0.44f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.49f, 0.49f, 0.49f, 0.25f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.95f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
		colors[ImGuiCol_Header] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.49f, 0.49f, 0.49f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.95f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.49f, 0.49f, 0.49f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		StyleTabColors(colors);

	}
	
	void SetStyle()
	{
		ImGui::PushItemWidth(130);
		if (ImGui::Combo("Colors", &Settings[style_color].Value.iValue, "Red\0Orange\0Yellow\0Green\0Blue\0Purple\0Aesir\0NeoGenesys\0Maya\0"))
		{
			switch (Settings[style_color].Value.iValue)
			{
			case 0: StyleColorsRed(); break;
			case 1: StyleColorsOrange(); break;
			case 2: StyleColorsYellow(); break;
			case 3: StyleColorsGreen(); break;
			case 4: StyleColorsBlue(); break;
			case 5: StyleColorsPurple(); break;
			case 6: StyleColorsAesir(); break;
			case 7: StyleColorsNeoGenesys(); break;
			case 8: StyleColorsMaya(); break;
			}
		}

		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::Combo("Fonts", &Settings[style_font].Value.iValue, "Tahoma\0Bank Gothic\0"))
		{
			io.FontDefault = io.Fonts->Fonts[Settings[style_font].Value.iValue];
		}


		ImGui::PopItemWidth();

		ImGui::Spacing();

		ImGui::Checkbox("Rainbow effect for the background", &Settings[style_rainbow].Value.bValue);
		
	}

	

	/************************************************************************/
	/* Load & Save Settings                                                 */
	/************************************************************************/

	static bool LoadWindowsSettings()
	{
		ImGuiContext& g = *GImGui;

		//Load Pos, Size
		for (int i = 0; i != g.Windows.Size; i++)
		{
			ImGuiWindow* window = g.Windows[i];
			const char* name = window->Name;
			{
				if (strstr(name, ALASKA17))
				{
					window->Pos = Settings[window_main_pos].Value.v2Value;
					window->SizeFull = ImMax(Settings[window_main_size].Value.v2Value, GImGui->Style.WindowMinSize);
				}
				else if (strstr(name, "Info"))
				{
					window->Pos = Settings[window_info_pos].Value.v2Value;
					window->SizeFull = ImMax(Settings[window_info_size].Value.v2Value, GImGui->Style.WindowMinSize);
				}
				else if (strstr(name, "Radar"))
				{
					window->Pos = Settings[window_radar_pos].Value.v2Value;
					window->SizeFull = ImMax(Settings[window_radar_size].Value.v2Value, GImGui->Style.WindowMinSize);
				}
				else if (strstr(name, "Console"))
				{
					window->Pos = Settings[window_console_pos].Value.v2Value;
					window->SizeFull = ImMax(Settings[window_console_size].Value.v2Value, GImGui->Style.WindowMinSize);
				}
			}
		}

		//Load color
		switch (Settings[style_color].Value.iValue)
		{
		case 0: StyleColorsRed(); break;
		case 1: StyleColorsOrange(); break;
		case 2: StyleColorsYellow(); break;
		case 3: StyleColorsGreen(); break;
		case 4: StyleColorsBlue(); break;
		case 5: StyleColorsPurple(); break;
		case 6: StyleColorsAesir(); break;
		case 7: StyleColorsNeoGenesys(); break;
		case 8: StyleColorsMaya(); break;
		}

		//Load font
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->Fonts[Settings[style_font].Value.iValue];

		return true;
	}

	static bool SaveWindowsSettings()
	{
		ImGuiContext& g = *GImGui;

		for (int i = 0; i != g.Windows.Size; i++)
		{
			ImGuiWindow* window = g.Windows[i];

			if (window->Flags & ImGuiWindowFlags_NoSavedSettings)
				continue;
			if (window->Pos.x == FLT_MAX)
				continue;
			const char* name = window->Name;
			if (strstr(name, "#"))
				continue;

			if (strstr(name, ALASKA17))
			{
				Settings[window_main_pos].Value.v2Value = window->Pos;
				Settings[window_main_size].Value.v2Value = window->SizeFull;
			}
			else if (strstr(name, "Info"))
			{
				Settings[window_info_pos].Value.v2Value = window->Pos;
				Settings[window_info_size].Value.v2Value = window->SizeFull;
			}
			else if (strstr(name, "Radar"))
			{
				Settings[window_radar_pos].Value.v2Value = window->Pos;
				Settings[window_radar_size].Value.v2Value = window->SizeFull;
			}
			else if (strstr(name, "Console"))
			{
				Settings[window_console_pos].Value.v2Value = window->Pos;
				Settings[window_console_size].Value.v2Value = window->SizeFull;
			}
		}

		return true;
	}





	/************************************************************************/
	/* ImGUI: MyConsole                                                     */
	/************************************************************************/

	class MyConsole
	{
	public:
		char                  InputBuf[256];
		ImVector<char*>       Items;
		bool                  ScrollToBottom;
		ImVector<char*>       History;
		int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
		ImVector<const char*> Commands;

		MyConsole() {}
		~MyConsole()
		{
			ClearLog();
			for (int i = 0; i < History.Size; i++)
				free(History[i]);
		}

		void Init()
		{
			static bool firstTime = false;
			if (!firstTime)
			{
				ClearLog();
				memset(InputBuf, 0, sizeof(InputBuf));
				HistoryPos = -1;

				Commands.push_back("clear");
				Commands.push_back("history");
				Commands.push_back("rc_crash");
				//Commands.push_back("rc_maprestart");
				//Commands.push_back("rc_scorelimit");
				//Commands.push_back("rc_timelimit");
				Commands.push_back("rc_name");
				Commands.push_back("rc_xp");
				Commands.push_back("rc_prestige");
				//Commands.push_back("rc_tokens");
				//Commands.push_back("rc_31");
				
				Commands.push_back("rc_unlock");
				Commands.push_back("rc_reset");

				//Commands.push_back("rc_derank");
				//Commands.push_back("rc_rerank");
				Commands.push_back("disconnect");
				Commands.push_back("quit");
							

				/*for (int i = 0; i < Commands.Size; i++)
				AddLog("Command[%i]: %s", i, Commands[i]);*/

				AddLog("Ready.");

				firstTime = true;
			}

		}


		// Portable helpers
		static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
		static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
		static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }

		void    ClearLog()
		{
			for (int i = 0; i < Items.Size; i++)
				free(Items[i]);
			Items.clear();
			ScrollToBottom = true;
		}

		void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			// FIXME-OPT
			char buf[1024];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
			buf[IM_ARRAYSIZE(buf) - 1] = 0;
			va_end(args);
			Items.push_back(Strdup(buf));
			ScrollToBottom = true;
		}

		void    Draw(const char* title, bool* p_open)
		{		
			// (Load Saved Window Settings)....
			static bool bFirstTime4Console = false;
			if (!bFirstTime4Console)
			{				
				LoadWindowsSettings();
				bFirstTime4Console = true;
			}

			ImGui::SetNextWindowPos(Settings[window_console_pos].Value.v2Value, ImGuiSetCond_Appearing);

			if (!ImGui::Begin(title, p_open, Settings[window_console_size].Value.v2Value, -1.0f, ImGuiWindowFlags_NoCollapse))
			{
				ImGui::End();
				return;
			}

			// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
			// Here we create a context menu only available from the title bar.
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Close"))
					*p_open = false;
				ImGui::EndPopup();
			}


			ImGui::TextWrapped("\t\t\tRhino Cheats");
			//ImGui::TextWrapped("\twww.sonderbytes.com");
			ImGui::Spacing();
			ImGui::TextWrapped("Press Help for details, press Tab to use text completion.");

			// TODO: display items starting from the bottom

			//if (ImGui::SmallButton("Add Dummy Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
			//if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); } ImGui::SameLine();

			if (ImGui::SmallButton("Help")) {
				AddLog("1. rc_crash\n\t\tCrashes everyone at the current match except you.");
				//AddLog("2. rc_maprestart\n\t\tRestart the current map. (as host).");
				//AddLog("3. rc_scorelimit <value>\n\t\tSet the limit for the score, 0 is unlimited. (as host).");
				//AddLog("4. rc_timelimit <value>\n\t\tSet the limit for the time, 0 is unlimited. (as host).");
				AddLog("2. rc_name <value>\n\t\tChanges your name.");
				AddLog("3. rc_xp <max|number>\n\t\tGives you the xp you need.");
				AddLog("4. rc_prestige <max|number>\n\t\tGives you the prestige you need.");
				//AddLog("8. rc_tokens <max|number>\n\t\tGives you the tokens you need.");
				//AddLog("9. rc_31\n\t\tLevel up primary weapons to 31, secondary weapons to 10, unlock pro perks.");

				AddLog("5. rc_unlock\n\t\tUnlock everything for you. (level 70, prestige 10, properks...)");
				AddLog("6. rc_reset\n\t\tReset everything for you. (level 1, prestige 0)");

				//AddLog("10. rc_derank <id>\n\t\tDerank the player by id, (without id): Show the player list.");
				//AddLog("11. rc_rerank <id>\n\t\tRerank the player by id, (without id): Show the player list.");
				AddLog("7. disconnect\n\t\tDisconnect from the game.");
				AddLog("8. quit\n\t\tQuit the game.");

			}ImGui::SameLine();
			if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
			bool copy_to_clipboard = ImGui::SmallButton("Copy"); //ImGui::SameLine();
																 //if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
																 //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

			ImGui::Separator();

			/*ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			static ImGuiTextFilter filter;
			filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
			ImGui::PopStyleVar();
			ImGui::Separator();*/

			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Clear")) ClearLog();
				ImGui::EndPopup();
			}

			// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
			// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
			// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
			// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
			//     ImGuiListClipper clipper(Items.Size);
			//     while (clipper.Step())
			//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			// However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
			// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
			// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
			// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			if (copy_to_clipboard)
				ImGui::LogToClipboard();
			ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			for (int i = 0; i < Items.Size; i++)
			{
				const char* item = Items[i];
				/*if (!filter.PassFilter(item))
				continue;*/
				ImVec4 col = col_default_text;
				if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
				else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.50f, 0.3f, 1.0f);
				ImGui::PushStyleColor(ImGuiCol_Text, col);
				ImGui::TextUnformatted(item);
				ImGui::PopStyleColor();
			}
			if (copy_to_clipboard)
				ImGui::LogFinish();
			if (ScrollToBottom)
				ImGui::SetScrollHere();
			ScrollToBottom = false;
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();

			// Command-line
			bool reclaim_focus = false;
			ImGui::PushItemWidth(-60);
			if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
			{
				char* input_end = InputBuf + strlen(InputBuf);
				while (input_end > InputBuf && input_end[-1] == ' ') { input_end--; } *input_end = 0;
				if (InputBuf[0])
					ExecCommand(InputBuf);
				strcpy(InputBuf, "");
				reclaim_focus = true;
			}
			ImGui::PopItemWidth();

			// Demonstrate keeping focus on the input box
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

			ImGui::End();			
		}

		typedef struct
		{
			int argNum;
			char cmdName[256];
			char cmdArgs[256][256];
		} cmdline_t;

		void SplitCommandLine(const char *inCMDLine, cmdline_t *outInfo)
		{
			memset(outInfo, 0, sizeof(cmdline_t));

			int len = strlen(inCMDLine) + 1;
			char *cmdbuf = new char[len];
			strcpy_s(cmdbuf, len, inCMDLine);

			char *token = strtok(cmdbuf, " ");
			while (token != nullptr)
			{
				if (outInfo->argNum == 0)
					strcpy_s(outInfo->cmdName, token);
				else
					strcpy_s(outInfo->cmdArgs[outInfo->argNum - 1], token);

				token = strtok(nullptr, " ");
				if (token != nullptr) ++outInfo->argNum;
			}

			delete[] cmdbuf;
		}

		void    ExecCommand(const char* command_line)
		{
			AddLog("# %s\n", command_line);

			// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.

			// Process command
			cmdline_t Output;
			SplitCommandLine(command_line, &Output);

			HistoryPos = -1;
			for (int i = History.Size - 1; i >= 0; i--)
				if (Stricmp(History[i], command_line) == 0)
				{
					free(History[i]);
					History.erase(History.begin() + i);
					break;
				}
			History.push_back(Strdup(command_line));


			if (Stricmp(Output.cmdName, "clear") == 0)
			{
				ClearLog();
			}
			else if (Stricmp(command_line, "history") == 0)
			{
				int first = History.Size - 10;
				for (int i = first > 0 ? first : 0; i < History.Size; i++)
					AddLog("%3d: %s\n", i, History[i]);
			}
			else if (Stricmp(Output.cmdName, "rc_crash") == 0)
			{
				AddLog("rc_crash executed.");
				Engine.SendToConsole("say \"\x5E\x02\xFF\xFF\xFF\"");
				AddLog("rc_crash done.");
			}
// 			else if (Stricmp(Output.cmdName, "rc_maprestart") == 0)
// 			{
// 				/*AddLog("rc_maprestart executed.");
// 				if (!*(int*)Offsets::host)
// 				{
// 					AddLog("[error] you need to be host.");
// 				}
// 				else
// 				{
// 					//0056EF50   83EC 44          SUB ESP, 0x44
// 					DWORD dwMr = 0x56F050;
// 					__asm
// 					{
// 						push 0
// 						push 0
// 						call dwMr
// 						add esp, 0x08
// 					}
// 				}
// 				AddLog("rc_maprestart done.");*/
// 			}
// 			else if (Stricmp(Output.cmdName, "rc_scorelimit") == 0)
// 			{
// 				/*AddLog("rc_scorelimit executed.");
// 
// 				if (!*(int*)Offsets::host)
// 				{
// 					AddLog("[error] you need to be host.");
// 				}
// 				else
// 				{
// 					int limitValue = atoi(Output.cmdArgs[0]);
// 					if (limitValue >= 0 && limitValue <= 999)
// 					{
// 						static char svBuf[50];
// 						dvar_s *g_gametype = Engine.FindVar("g_gametype");
// 						XASSERT(g_gametype);
// 						sprintf_s(svBuf, "scr_%s_scorelimit %i", g_gametype->u6.sval, limitValue);
// 						Engine.SendToConsole(svBuf);
// 					}
// 					else
// 					{
// 						AddLog("[error] the number need to be >= 0 and <= 999.");
// 					}
// 				}
// 
// 				AddLog("rc_scorelimit done.");*/
// 			}
// 			else if (Stricmp(Output.cmdName, "rc_timelimit") == 0)
// 			{
// 				/*AddLog("rc_timelimit executed.");
// 
// 				if (!*(int*)Offsets::host)
// 				{
// 					AddLog("[error] you need to be host.");
// 				}
// 				else
// 				{
// 					int limitValue = atoi(Output.cmdArgs[0]);
// 					if (limitValue >= 0 && limitValue <= 999)
// 					{
// 						static char svBuf[50];
// 						dvar_s *g_gametype = Engine.FindVar("g_gametype");
// 						XASSERT(g_gametype);
// 
// 						sprintf_s(svBuf, "scr_%s_timelimit %i", g_gametype->u6.sval, limitValue);
// 						Engine.SendToConsole(svBuf);
// 					}
// 					else
// 					{
// 						AddLog("[error] the number need to be >= 0 and <= 999.");
// 					}
// 				}
// 
// 				AddLog("rc_timelimit done.");*/
// 			}
			else if (Stricmp(Output.cmdName, "rc_name") == 0)
			{
				AddLog("rc_name executed.");

				char abuf[512];
				memset(abuf, 0, 512);
				for (int i = 0; i < Output.argNum; i++)
				{
					char tabuf[256];
					sprintf_s(tabuf, "%s ", Output.cmdArgs[i]);
					memcpy(abuf + strlen(abuf), tabuf, strlen(tabuf) + 1);
				}

				char *newName = strtok(abuf, "\n");
				if (newName)
				{
					//strcpy((char*)Offsets::namegame, newName);
					//Engine.SendToConsole("Name Let's go!");
					//AddLog("Your name is now: %s", (char*)Offsets::namegame);

					std::string toSend = "userinfo \"";
					toSend += "\\name\\"; toSend += newName;
					toSend += "\"";
					Engine.SendToConsole((char*)(toSend.c_str()));
				}

				AddLog("rc_name done.");
			}
			else if (Stricmp(Output.cmdName, "rc_xp") == 0)
			{
				AddLog("rc_xp executed.");

				if (strstr(Output.cmdArgs[0], "max"))
				{
					pStats->xp = 2516000;
					AddLog("Max value set");
				}
				else
				{
					int xpvalue = atoi(Output.cmdArgs[0]);
					if (xpvalue >= 0 && xpvalue <= 2516000)
					{
						pStats->xp = xpvalue;
					}
					else
					{
						AddLog("[error] the number need to be >= 0 and <= 2516000.");
					}
				}
				AddLog("rc_xp done.");
			}
			else if (Stricmp(Output.cmdName, "rc_prestige") == 0)
			{
				AddLog("rc_prestige executed.");

				if (strstr(Output.cmdArgs[0], "max"))
				{
					pStats->prestige = 10;
					AddLog("Max value set");
				}
				else
				{
					int prestigevalue = atoi(Output.cmdArgs[0]);
					if (prestigevalue >= 0 && prestigevalue <= 10)
					{
						pStats->prestige = prestigevalue;
					}
					else
					{
						AddLog("[error] the number need to be >= 0 and <= 10.");
					}
				}

				AddLog("rc_prestige done.");
			}			
			else if (Stricmp(Output.cmdName, "quit") == 0)
			{
				exit(-1);
			}
			else if (Stricmp(Output.cmdName, "disconnect") == 0)
			{
				//Should work but it doesn't :(
				//disconnect();

				Engine.SendToConsole("disconnect");

				AddLog("Disconnected.");
			}
			


			//Derank-Rerank Here...


			else if (Stricmp(Output.cmdName, "rc_unlock") == 0)
			{
				AddLog("sh_unlock executed.");

				pStats->xp = 2516000;
				pStats->prestige = 10;

				for (int i = 0; i < 284; i++)
				{
					if (i < 16) pStats->perkChallenges[i] = 11;
					if (i < 187) pStats->miscChallenges[i] = 11;
					pStats->weaponChallenges[i] = 11;
				}

				AddLog("sh_unlock done.");
			}
			else if (Stricmp(Output.cmdName, "rc_reset") == 0)
			{
				AddLog("sh_reset executed.");

				pStats->xp = 0;
				pStats->prestige = 0;

				for (int i = 0; i < 284; i++)
				{
					if (i < 16) pStats->perkChallenges[i] = 0;
					if (i < 187) pStats->miscChallenges[i] = 0;
					pStats->weaponChallenges[i] = 0;
				}


				AddLog("sh_reset done.");
			}
						
			else
			{
				AddLog("Unknown command: '%s'\n", command_line);
			}
		}

		static int TextEditCallbackStub(ImGuiTextEditCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
		{
			MyConsole* console = (MyConsole*)data->UserData;
			return console->TextEditCallback(data);
		}

		int     TextEditCallback(ImGuiTextEditCallbackData* data)
		{
			//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackCompletion:
			{
				// Example of TEXT COMPLETION

				// Locate beginning of current word
				const char* word_end = data->Buf + data->CursorPos;
				const char* word_start = word_end;
				while (word_start > data->Buf)
				{
					const char c = word_start[-1];
					if (c == ' ' || c == '\t' || c == ',' || c == ';')
						break;
					word_start--;
				}

				// Build a list of candidates
				ImVector<const char*> candidates;
				for (int i = 0; i < Commands.Size; i++)
					if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
						candidates.push_back(Commands[i]);

				if (candidates.Size == 0)
				{
					// No match
					AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
				}
				else if (candidates.Size == 1)
				{
					// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0]);
					data->InsertChars(data->CursorPos, " ");
				}
				else
				{
					// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
					int match_len = (int)(word_end - word_start);
					for (;;)
					{
						int c = 0;
						bool all_candidates_matches = true;
						for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
							if (i == 0)
								c = toupper(candidates[i][match_len]);
							else if (c == 0 || c != toupper(candidates[i][match_len]))
								all_candidates_matches = false;
						if (!all_candidates_matches)
							break;
						match_len++;
					}

					if (match_len > 0)
					{
						data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
						data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
					}

					// List matches
					AddLog("Possible matches:\n");
					for (int i = 0; i < candidates.Size; i++)
						AddLog("- %s\n", candidates[i]);
				}

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				// Example of HISTORY
				const int prev_history_pos = HistoryPos;
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (HistoryPos == -1)
						HistoryPos = History.Size - 1;
					else if (HistoryPos > 0)
						HistoryPos--;
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (HistoryPos != -1)
						if (++HistoryPos >= History.Size)
							HistoryPos = -1;
				}

				// A better implementation would preserve the data on the current input line along with cursor position.
				if (prev_history_pos != HistoryPos)
				{
					data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (HistoryPos >= 0) ? History[HistoryPos] : "");
					data->BufDirty = true;
				}
			}
			}
			return 0;
		}
	};

	MyConsole console; //<<< new way

	static void ShowConsole(bool* p_open)
	{
		//static MyConsole console;
		console.Init();
		console.Draw("Console", p_open);
	}

	
	/************************************************************************/
	/* Continue...                                                          */
	/************************************************************************/

	void SetTextureColor(IDirect3DTexture9 *texture, DWORD color)
	{
		if (texture == nullptr)
			return;

		D3DSURFACE_DESC desc;
		texture->GetLevelDesc(0, &desc);
		RECT t;
		t.left = 0;
		t.right = desc.Width;
		t.top = 0;
		t.bottom = desc.Height;
		D3DLOCKED_RECT p;
		texture->LockRect(0, &p, &t, D3DLOCK_DISCARD);

		if (p.pBits == nullptr)
			return;

		for (int i = 0; i < desc.Width; i++)
			for (int j = 0; j < desc.Height; j++)
				*reinterpret_cast<PDWORD>(reinterpret_cast<DWORD>(p.pBits) + i * 4 + j * p.Pitch) = color;

		texture->UnlockRect(0);
	}
	
	void GUI_Init(IDirect3DDevice9* pDevice)
	{
		if (g_hWindow && pDevice)
		{
			D3DXCreateTexture(pDevice, 1, 1, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texAxis);
			D3DXCreateTexture(pDevice, 1, 1, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texAxisVisible);
			D3DXCreateTexture(pDevice, 1, 1, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texAllies);
			D3DXCreateTexture(pDevice, 1, 1, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texAlliesVisible);
			D3DXCreateTexture(pDevice, 1, 1, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texWeapon);
			D3DXCreateTexture(pDevice, 1, 1, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texWeaponVisible);


			std::string texfile = texturesFolder;
			texfile += "texture-1.png";
			D3DXCreateTextureFromFile(pDevice, texfile.c_str(), &texWeaponCustom);
			if (!texWeaponCustom)
			{
				texfile = texturesFolder;
				texfile += "texture-1.jpg";
				D3DXCreateTextureFromFile(pDevice, texfile.c_str(), &texWeaponCustom);
				if (!texWeaponCustom)
				{				
					SafeMessageBox(HWND_DESKTOP, 
						STREAM(texfile << " not found.\nRemember that .jpg extension is also allowed.\nUsing normal textures.").c_str(), ALASKA17, MB_ICONWARNING);
				}
			}

			texfile = texturesFolder;
			texfile += "texture-map.png";
			D3DXCreateTextureFromFile(pDevice, texfile.c_str(), &texMap);
			if (!texMap)
			{
				texfile = texturesFolder;
				texfile += "texture-map.jpg";
				D3DXCreateTextureFromFile(pDevice, texfile.c_str(), &texMap);
				if (!texMap)
				{					
					SafeMessageBox(HWND_DESKTOP, 
						STREAM(texfile << " not found.\nRemember that .jpg extension is also allowed.").c_str(), ALASKA17, MB_ICONWARNING);
				}
			}
			
			ImGui::CreateContext();

			//no imgui.ini
			ImGuiIO& io = ImGui::GetIO();
			io.IniFilename = nullptr;

			//default style
			StyleColorsAesir();

			ImGui_ImplWin32_Init(g_hWindow);
			ImGui_ImplDX9_Init(pDevice);	
			
			//io.FontDefault = io.Fonts->Fonts[1];

			g_bWasInitialized = true;
		}
	}

	void Restore_WndProc()
	{
		if (g_hWindow && g_pOldWindowProc)
		{
			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			//Restore the WindowProc
			SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_pOldWindowProc));
		}


		if (pD3DSprite)
		{
			pD3DSprite->Release();
			pD3DSprite = nullptr;
		}
	}


	/************************************************************************/
	/* Drawing frames                                                       */
	/************************************************************************/

	void DrawRadar()
	{
		if (Settings[esp_radar].Value.bValue)
		{
			ImGui::SetNextWindowPos(ImVec2(Settings[esp_radar_x].Value.fValue, Settings[esp_radar_y].Value.fValue), ImGuiSetCond_Appearing);

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.15f));

			ImGui::Begin("Radar", &Settings[esp_radar].Value.bValue, ImVec2(Settings[esp_radar_w].Value.fValue, Settings[esp_radar_h].Value.fValue), -1.0f, ImGuiWindowFlags_NoCollapse);
			{
				if (ImGui::IsWindowFocused)
				{
					ImVec2 windowPos = ImGui::GetWindowPos();
					Settings[esp_radar_x].Value.fValue = windowPos.x;
					Settings[esp_radar_y].Value.fValue = windowPos.y;

					ImVec2 windowSize = ImGui::GetWindowSize();
					Settings[esp_radar_w].Value.fValue = windowSize.x;
					Settings[esp_radar_h].Value.fValue = windowSize.y;
				}

				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				if (draw_list != nullptr)
				{
					draw_list->AddLine(
						ImVec2(Settings[esp_radar_x].Value.fValue + (Settings[esp_radar_w].Value.fValue / 2), Settings[esp_radar_y].Value.fValue),
						ImVec2(Settings[esp_radar_x].Value.fValue + (Settings[esp_radar_w].Value.fValue / 2), Settings[esp_radar_y].Value.fValue + Settings[esp_radar_h].Value.fValue), ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]), 2.0f);


					draw_list->AddLine(
						ImVec2(Settings[esp_radar_x].Value.fValue, Settings[esp_radar_y].Value.fValue + (Settings[esp_radar_h].Value.fValue / 2)),
						ImVec2(Settings[esp_radar_x].Value.fValue + Settings[esp_radar_w].Value.fValue, Settings[esp_radar_y].Value.fValue + (Settings[esp_radar_h].Value.fValue / 2)), ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]), 2.0f);
				}
			}
			ImGui::PopStyleColor();
			ImGui::End();

			Radar.rw = Settings[esp_radar_w].Value.fValue;
			Radar.rh = Settings[esp_radar_h].Value.fValue; // -25 to Fit on Imgui Radar Y

			// (Load Saved Window Settings)....
			static bool bFirstTime4Radar = false;
			if (!bFirstTime4Radar)
			{				
				LoadWindowsSettings();
				bFirstTime4Radar = true;
			}
		}
	}

	void DrawMenu() {

		static bool menu_flag = 1;
		static bool menu_turn_off = 0;

		/*static bool di = true;
		ImGui::Begin("Menu Info", &di, ImVec2(350, 350), -1.0f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		{
		ImGui::Text("aSize X: %f, Size Y: %f", aimsize.x, aimsize.y); //426, 248
		ImGui::Text("eSize X: %f, Size Y: %f", espsize.x, espsize.y); //426, 435
		ImGui::Text("dSize X: %f, Size Y: %f", d3dsize.x, d3dsize.y); //.. 225
		ImGui::Text("mSize X: %f, Size Y: %f", miscsize.x, miscsize.y); // 595
		ImGui::Text("hSize X: %f, Size Y: %f", hostsize.x, hostsize.y); // 327
		ImGui::Text("kSize X: %f, Size Y: %f", keysize.x, keysize.y); // 327
		ImGui::Text("iSize X: %f, Size Y: %f", infoSize.x, infoSize.y); // 327
		ImGui::Text("MSize X: %f, Size Y: %f", msize.x, msize.y); // 327

		}
		ImGui::End();*/

		//Enable/Disable Mouse
		{
			tagPOINT lp;
			GetCursorPos(&lp);

			static bool bSetValues = false;				
			static DWORD dwRestore = 0;
			static int iOldMousePos[] = { 0, 0 };

			if (!bSetValues && g_bMainWindowOpen)
			{
				dwRestore = *(PDWORD)Offsets::mouse;
				*(PDWORD)Offsets::mouse = 0;
		

				iOldMousePos[0] = lp.x;
				iOldMousePos[1] = lp.y;

				bSetValues = true;
			}
			else if (bSetValues && !g_bMainWindowOpen)
			{
				SetCursorPos(iOldMousePos[0], iOldMousePos[1]);

				*(PDWORD)Offsets::mouse = dwRestore;				

				bSetValues = false;
			}
		}

		if (g_bMainWindowOpen)
		{
			// (Load Saved Window Settings)....
			static bool bFirstTime4Menu = false;
			if (!bFirstTime4Menu)
			{						
				LoadWindowsSettings();
				bFirstTime4Menu = true;
			}

			if (menu_flag) {
				SafeCreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Hook.playSound), sounds.menu, 0, nullptr);
				menu_flag = 0;
			}

			if (!menu_turn_off)
				menu_turn_off = 1;

			ImGui::SetNextWindowPos(Settings[window_main_pos].Value.v2Value, ImGuiSetCond_Appearing);

			ImGui::Begin(ALASKA17, &g_bMainWindowOpen, Settings[window_main_size].Value.v2Value, -1.0f, ImGuiWindowFlags_NoCollapse);
			{
				msize = ImGui::GetWindowSize();
				mpos = ImGui::GetWindowPos();

				float xSize = msize.x;
				if (xSize < 555)
					xSize = 555;
				{
					//static const char* tabTooltips[numTabs] = { "Render Tab Tooltip","","","","Object Type Tooltip" };
					static const char* tabNames[] = { "Aimbot", "Esp", "D3D", "Misc", "Host", "Keys", "Style" };
					static const int numTabs = sizeof(tabNames) / sizeof(tabNames[0]);
					static int tabItemOrdering[numTabs] = { 0, 1, 2, 3, 4, 5, 6 };
					static int selectedTab = 0;
					static int optionalHoveredTab = 0;
					const bool tabSelectedChanged = ImGui::TabLabels(numTabs, tabNames, selectedTab, /*tabTooltips*/nullptr, false, &optionalHoveredTab, &tabItemOrdering[0], false);

					ImGui::TabLabelStyle &tabStyle = ImGui::TabLabelStyle::Get();
					tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ImGui::ColorConvertFloat4ToU32(colRainBow);
					ImGuiStyle &style = ImGui::GetStyle();
					tabStyle.colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_HeaderHovered]);

					switch (tabItemOrdering[selectedTab])
					{
					case 0:
						ImGui::BeginChild("Aim Child", ImVec2(0, 194), true);
						{
							//aimsize = ImGui::GetWindowSize();

							ImGui::Checkbox("Enabled", &Settings[aim_enabled].Value.bValue);
							ImGui::Spacing();
							ImGui::Spacing();

							if (!Settings[aim_enabled].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}

							{
								ImGui::PushItemWidth(100);
								ImGui::Combo("By Key", &Settings[aim_key].Value.iValue, "Off\0Mouse 1\0Mouse 2\0Mouse 3\0Ctrl\0Shift\0");
								ImGui::PopItemWidth();

								if (Settings[aim_mode].Value.iValue)
								{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}
								ImGui::SameLine(xSize - 390);
								ImGui::PushItemWidth(150);
								ImGui::SliderFloat("Fov", &Settings[fov].Value.fValue, 0.0f, 360.0f);
								ImGui::PopItemWidth();
								if (Settings[aim_mode].Value.iValue)
								{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
								}

								ImGui::PushItemWidth(100);
								ImGui::Combo("Mode", &Settings[aim_mode].Value.iValue, "Fov\0Distance\0");
								ImGui::PopItemWidth();

								if (!Settings[aim_mode].Value.iValue)
								{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}
								ImGui::SameLine(xSize - 390);

								ImGui::PushItemWidth(100);

								ImGui::Combo("Autowall", &Settings[auto_wall].Value.iValue, "Off\0Simulated\0Non Simulated\0");
								if (!Settings[aim_mode].Value.iValue)
								{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
								}

								ImGui::Spacing();
								ImGui::Checkbox("Auto shoot", &Settings[auto_shoot].Value.bValue);

								ImGui::SameLine(xSize - 390);
								ImGui::Checkbox("Head only", &Settings[head_only].Value.bValue);

								ImGui::Checkbox("Auto knife", &Settings[auto_knife].Value.bValue);

								ImGui::SameLine(xSize - 390);
								ImGui::Checkbox("Prediction", &Settings[aim_prediction].Value.bValue);
								ImGui::Checkbox("Anti anti aim", &Settings[anti_antiaim].Value.bValue);

								ImGui::SameLine(xSize - 390);
								ImGui::Checkbox("Enemies with riot shield", &Settings[enemies_with_riotshield].Value.bValue);


								if (ImGui::Checkbox("Team killer", &Settings[team_killer].Value.bValue))
								{
									if (Settings[team_killer].Value.bValue)
										ImGui::OpenPopup("Team Killer");
								}

								//ImGui::SameLine(xSize - 390);
								//ImGui::Checkbox("Auto QickScope", &bAutoQuickScope);								


								if (ImGui::BeginPopupModal("Team Killer", NULL, ImGuiWindowFlags_AlwaysAutoResize))
								{
									ImGui::Text("Remember to switch to the other team first\nF2: Switch team\n\n");
									ImGui::Separator();

									if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }

									ImGui::EndPopup();
								}

								/*ImGui::SameLine(xSize - 390);
								ImGui::Checkbox("Ignore enemies controlling devices", &bValue);*/

								/*ImGui::Spacing();
								ImGui::Checkbox("By Priority", &bValue);*/
							}


							if (!Settings[aim_enabled].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}
						}
						ImGui::EndChild();
						break;

					case 1:
						ImGui::BeginChild("ESP Child", ImVec2(0, 371), true);
						{
							//espsize = ImGui::GetWindowSize();

							ImGui::Checkbox("Enabled", &Settings[esp_enabled].Value.bValue);
							ImGui::Spacing();
							ImGui::Spacing();

							if (!Settings[esp_enabled].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}

							{
								ImGui::Checkbox("Enemies", &Settings[esp_enemy].Value.bValue);
								if (!Settings[esp_enemy].Value.bValue)
								{

									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}
								{
									ImGui::SameLine(xSize - 390);
									ImGui::ColorCombo("Enemy   ", &Settings[esp_enemy_color].Value.v4Value, false, 100.0f, false);
									ImGui::SameLine(xSize - 270);
									ImGui::ColorCombo("Visible ", &Settings[esp_visible_color].Value.v4Value, false, 100, false);
								}
								if (!Settings[esp_enemy].Value.bValue)
								{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
								}


								ImGui::Checkbox("Friendlies", &Settings[esp_friendly].Value.bValue);
								if (!Settings[esp_friendly].Value.bValue)
								{

									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}
								{
									ImGui::SameLine(xSize - 390);
									ImGui::ColorCombo("Friendly", &Settings[esp_friendly_color].Value.v4Value, false, 100.0f, false);
								}
								if (!Settings[esp_friendly].Value.bValue)
								{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
								}


								ImGui::Checkbox("Name", &Settings[esp_name].Value.bValue);
								if (!Settings[esp_name].Value.bValue)
								{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}
								{
									ImGui::Spacing(); ImGui::SameLine(25);
									ImGui::Checkbox("Boxed", &Settings[esp_name_boxed].Value.bValue);
								}
								if (!Settings[esp_name].Value.bValue)
								{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
								}


								ImGui::Checkbox("Weapon", &Settings[esp_weapon].Value.bValue);
								ImGui::SameLine(xSize - 350); ImGui::Checkbox("Bright colors", &Settings[esp_bright_colors].Value.bValue);
								ImGui::Checkbox("Skeleton", &Settings[esp_conected_bones].Value.bValue);

								ImGui::Spacing();

								ImGui::Checkbox("Misc esp", &Settings[esp_misc].Value.bValue);
								if (!Settings[esp_misc].Value.bValue)
								{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}
								{
									ImGui::Spacing(); ImGui::SameLine(25);
									ImGui::Checkbox("Items", &Settings[esp_misc_items].Value.bValue);

									/*if (!Settings[esp_misc_items].Value.bValue)
									{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
									}*/
									{
										ImGui::SameLine(xSize - 350);
										ImGui::ColorCombo("Item     ", &Settings[esp_misc_items_color].Value.v4Value, false, 115, false);
									}
									/*if (!Settings[esp_misc_items].Value.bValue)
									{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
									}*/



									ImGui::Spacing(); ImGui::SameLine(25);
									ImGui::Checkbox("Devices", &Settings[esp_misc_devices].Value.bValue);
									/*if (!Settings[esp_misc_devices].Value.bValue)
									{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
									}*/
									{
										ImGui::SameLine(xSize - 350);
										ImGui::ColorCombo("Device   ", &Settings[esp_misc_devices_color].Value.v4Value, false, 115, false);
									}
									/*if (!Settings[esp_misc_devices].Value.bValue)
									{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
									}*/


									ImGui::Spacing(); ImGui::SameLine(25);
									ImGui::Checkbox("Explosives", &Settings[esp_misc_explosives].Value.bValue);
									/*if (!Settings[esp_misc_explosives].Value.bValue)
									{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
									}*/
									{
										ImGui::SameLine(xSize - 350);
										ImGui::ColorCombo("Explosive", &Settings[esp_misc_explosives_color].Value.v4Value, false, 115, false);
									}
									/*if (!Settings[esp_misc_explosives_color].Value.bValue)
									{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
									}*/
								}
								if (!Settings[esp_misc].Value.bValue)
								{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
								}


								ImGui::Spacing();

								ImGui::Checkbox("Radar", &Settings[esp_radar].Value.bValue);
								ImGui::SameLine(xSize - 390);
								ImGui::Checkbox("Compass", &Settings[esp_compass_radar].Value.bValue);

								if (!Settings[esp_compass_radar].Value.bValue)
								{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}
								{
									ImGui::Spacing();
									ImGui::SameLine(xSize - 390 + 17);
									ImGui::PushItemWidth(140);
									ImGui::SliderFloat("Compass's radius", &Settings[esp_compass_radar_radius].Value.fValue, 0.0f, 100.0f);
									ImGui::PopItemWidth();
								}
								if (!Settings[esp_compass_radar].Value.bValue)
								{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
								}


								ImGui::Spacing();

								static const char* boxes[] = { "Off", "Cornered", "Cornered filled", "2D", "2D filled", "3D", "Lilli's box" };
								ImGui::PushItemWidth(100);
								ImGui::Combo("Boxes", &Settings[esp_box].Value.iValue, boxes, IM_ARRAYSIZE(boxes));
								ImGui::PopItemWidth();

								ImGui::SameLine(xSize - 390);
								static const char* sls[] = { "Off", "Top center", "At crosshair", "Base center" };
								ImGui::PushItemWidth(100);
								ImGui::Combo("Snaplines", &Settings[esp_snap_lines].Value.iValue, sls, IM_ARRAYSIZE(sls));
								ImGui::PopItemWidth();
							}

							if (!Settings[esp_enabled].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}

						}
						ImGui::EndChild();
						break;

					case 2:
						ImGui::BeginChild("D3D Child", ImVec2(0, 292), true);
						{
							//d3dsize = ImGui::GetWindowSize();

							ImGui::Checkbox("Enabled", &Settings[d3d_enabled].Value.bValue);

							if (!Settings[d3d_enabled].Value.bValue)
							{

								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}

							{

								/*ImGui::SameLine(xSize - 390);
								ImGui::Checkbox("Trip map", &Settings[d3d_tripmap].Value.bValue);
								*/

								ImGui::Spacing();
								ImGui::Spacing();

								ImGui::Checkbox("Chams", &Settings[d3d_chams].Value.bValue);


								if (!Settings[d3d_chams].Value.bValue)
								{

									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
								}

								{
									ImGui::SameLine(xSize - 390);
									ImGui::Checkbox("Wireframe", &Settings[d3d_chams_wireframe].Value.bValue);


									ImGui::Spacing(); ImGui::SameLine(25);


									ImGui::Checkbox("Axis", &Settings[d3d_chams_axis].Value.bValue);
									/*if (!Settings[d3d_chams_axis].Value.bValue)
									{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
									}*/
									ImGui::SameLine(xSize - 390);
									ImGui::ColorCombo("Axis   ", &Settings[d3d_axis_color].Value.v4Value, false, 100, false);
									ImGui::SameLine(xSize - 270);
									ImGui::ColorCombo("Axis visible", &Settings[d3d_axis_visible_color].Value.v4Value, false, 100, false);
									/*if (!Settings[d3d_chams_axis].Value.bValue)
									{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
									}*/

									ImGui::Spacing(); ImGui::SameLine(25);
									ImGui::Checkbox("Allies", &Settings[d3d_chams_allies].Value.bValue);
									/*if (!Settings[d3d_chams_allies].Value.bValue)
									{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
									}*/
									ImGui::SameLine(xSize - 390);
									ImGui::ColorCombo("Allies ", &Settings[d3d_allies_color].Value.v4Value, false, 100, false);
									ImGui::SameLine(xSize - 270);
									ImGui::ColorCombo("Allies visible", &Settings[d3d_allies_visible_color].Value.v4Value, false, 100, false);
									/*if (!Settings[d3d_chams_allies].Value.bValue)
									{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
									}*/

									ImGui::Spacing();
									ImGui::Spacing(); ImGui::SameLine(25);

									ImGui::Checkbox("Weapons", &Settings[d3d_chams_weapons].Value.bValue);
									/*if (!Settings[d3d_chams_weapons].Value.bValue)
									{
									ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
									ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
									}*/
									ImGui::SameLine(xSize - 390);
									ImGui::ColorCombo("Weapons", &Settings[d3d_weapon_color].Value.v4Value, false, 100, false);
									ImGui::SameLine(xSize - 270);
									ImGui::ColorCombo("Weapons visible", &Settings[d3d_weapon_visible_color].Value.v4Value, false, 100, false);

									ImGui::Spacing(); ImGui::SameLine(42);
									ImGui::Checkbox("Custom texture", &Settings[d3d_weapon_customtexture].Value.bValue);

									/*if (!Settings[d3d_chams_weapons].Value.bValue)
									{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
									}*/
								}

								if (!Settings[d3d_chams].Value.bValue)
								{
									ImGui::PopItemFlag();
									ImGui::PopStyleVar();
								}

								ImGui::Checkbox("No sky", &Settings[d3d_nosky].Value.bValue);
								ImGui::Checkbox("No hands", &Settings[d3d_nohands].Value.bValue);
								ImGui::Checkbox("No weapon", &Settings[d3d_noweapon].Value.bValue);
								ImGui::Checkbox("No smoke", &Settings[d3d_nosmoke].Value.bValue);

								ImGui::Checkbox("Glass", &Settings[d3d_glass].Value.bValue);
							}


							if (!Settings[d3d_enabled].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}

						}
						ImGui::EndChild();
						break;

					case 3:
						ImGui::BeginChild("MISC Child", ImVec2(0, 453), true);
						{
							//miscsize = ImGui::GetWindowSize();

							ImGui::PushItemWidth(150);
							ImGui::SliderFloat("Field of view", &Settings[force_fov].Value.fValue, 65.0f, 200.0f);
							ImGui::PopItemWidth();
							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Show Console", &Settings[show_console].Value.bValue);


							ImGui::Checkbox("Fullbright", &Settings[fullbright].Value.bValue);
							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Show info", &Settings[show_info].Value.bValue);

							ImGui::PushItemWidth(150);
							ImGui::SliderFloat("Gun x", &Settings[gun_x].Value.fValue, 0.0f, 200.0f);
							ImGui::PopItemWidth();
							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Draw clock", &Settings[draw_clock].Value.bValue);

							ImGui::PushItemWidth(150);
							ImGui::SliderFloat("Gun y", &Settings[gun_y].Value.fValue, 0.0f, 200.0f);
							ImGui::PopItemWidth();
							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Name stealer", &Settings[names_stealer].Value.bValue);

							ImGui::PushItemWidth(150);
							ImGui::SliderFloat("Gun z", &Settings[gun_z].Value.fValue, 0.0f, 200.0f);
							ImGui::PopItemWidth();

							/*ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Drunken Style", &Settings[drunken_style].Value.bValue);*/

							ImGui::Checkbox("Kill spam", &Settings[kill_spam].Value.bValue);
							ImGui::Spacing();
							ImGui::SameLine(25);
							ImGui::PushItemWidth(250);

							if (!Settings[kill_spam].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}

							ImGui::InputText("Kill spam's text", &Settings[kill_spam_text].Value.szValue);
							if (!Settings[kill_spam].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}

							ImGui::PopItemWidth();
							ImGui::SameLine(); //ShowHelpMarker("Up to 250 chars.\nUse %s for victim's name\n");

							ImGui::Spacing();

							ImGui::Checkbox("No recoil", &Settings[no_recoil].Value.bValue);
							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Laser", &Settings[laser].Value.bValue);

							ImGui::Checkbox("No spread", &Settings[no_spread].Value.bValue);
							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Kill sounds", &Settings[kill_sounds].Value.bValue);


							ImGui::Checkbox("Silent aim", &Settings[silent_aim].Value.bValue);
							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Third person", &Settings[third_person].Value.bValue);

							ImGui::Checkbox("Inverse troller", &Settings[inverse_troller].Value.bValue);
							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("AFK mode", &Settings[afk_mode].Value.bValue);

							ImGui::Spacing();
							ImGui::Checkbox("Show kill icons", &Settings[kill_icons].Value.bValue);


							ImGui::Spacing();
							ImGui::SameLine(25);


							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}
							ImGui::Checkbox("Rotate", &Settings[kill_icons_rotate].Value.bValue);
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}



							ImGui::SameLine(xSize - 280);
							static const char* aas[] = { "Off", "Rotated", "Protected" };
							ImGui::PushItemWidth(90);
							ImGui::Combo("Anti aim mode", &Settings[anti_aim].Value.iValue, aas, IM_ARRAYSIZE(aas));
							ImGui::PopItemWidth();

							ImGui::Spacing();
							ImGui::SameLine(25);
							static const char* kits[] = { "HeadShot", "Skull", "Random" };
							ImGui::PushItemWidth(90);

							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}
							ImGui::Combo("Type", &Settings[kill_icons_type].Value.iValue, kits, IM_ARRAYSIZE(kits));
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}

							ImGui::PopItemWidth();

							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Rainbow mode", &Settings[rainbow_mode].Value.bValue);

							ImGui::Spacing();
							ImGui::SameLine(25);
							static const char* kios[] = { "Center", "Top left", "Top center", "Top right", "Left", "Right", "Bottom keft", "Bottom center", "Bottom right" };
							ImGui::PushItemWidth(90);
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}
							ImGui::Combo("Initial Position", &Settings[kill_icons_origin].Value.iValue, kios, IM_ARRAYSIZE(kios));
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}
							ImGui::PopItemWidth();

							ImGui::SameLine(xSize - 280);
							static const char* crosshairs[] = { "Off", "Normal", "Lilli" };
							ImGui::PushItemWidth(90);
							ImGui::Combo("Crosshair mode", &Settings[draw_crosshair].Value.iValue, crosshairs, IM_ARRAYSIZE(crosshairs));
							ImGui::PopItemWidth();

							ImGui::Spacing();
							ImGui::SameLine(25);
							static const char* kids[] = { "Left", "Top", "Right", "Bottom" };
							ImGui::PushItemWidth(90);
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}
							ImGui::Combo("Final Position", &Settings[kill_icons_destination].Value.iValue, kids, IM_ARRAYSIZE(kids));
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}
							ImGui::PopItemWidth();
							ImGui::Spacing();
							ImGui::SameLine(25);

							static const char* kiss[] = { "Small", "Normal", "Big" };
							ImGui::PushItemWidth(90);
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}
							ImGui::Combo("Size", &Settings[kill_icons_size].Value.iValue, kiss, IM_ARRAYSIZE(kiss));
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}
							ImGui::PopItemWidth();

							ImGui::SameLine(xSize - 280);
							ImGui::Checkbox("Watermark", &Settings[watermark].Value.bValue);

							ImGui::Spacing();
							ImGui::SameLine(25);
							ImGui::PushItemWidth(90);
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}
							ImGui::SliderInt("Quantity", &Settings[kill_icons_count].Value.iValue, 1, 10);
							if (!Settings[kill_icons].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}
							ImGui::PopItemWidth();


						}
						ImGui::EndChild();
						break;

					case 4:
						ImGui::BeginChild("HOST Child", ImVec2(0, 295), true);
						{
							//hostsize = ImGui::GetWindowSize();


							static bool doOnce = false;
							if (!doOnce)
							{
								doOnce = true;

								InitHostVars();
							}
							

							if (ImGui::Checkbox("Force host", &Settings[host_forcehost].Value.bValue))
							{
								static bool bNonHost = true;
								if (Settings[host_forcehost].Value.bValue && !*(int*)(Offsets::host) && bNonHost)
								{									
									m_connectTimeOut->current.integer = 1;
									m_bandwithTestEnabled->current.enabled = false;
									m_autoTeams->current.enabled = true;
									m_maxTeamDifference->current.integer = 18;
									m_requireOpenNat->current.enabled = false;
									*m_doWeSuckAsHost = false;
									*m_natType = NAT_OPEN;									

									Engine.SendToConsole("xblive_privatematch 1; ui_gametype dm; xpartygo; xblive_privatematch 0;\n");

									bNonHost = false;									
								}
								else if (!Settings[host_forcehost].Value.bValue && !bNonHost)
								{
									m_connectTimeOut->current.integer = m_connectTimeOut->reset.integer;
									m_bandwithTestEnabled->current.enabled = m_bandwithTestEnabled->reset.enabled;
									m_autoTeams->current.enabled = m_autoTeams->reset.enabled;
									m_maxTeamDifference->current.integer = m_maxTeamDifference->reset.integer;
									m_requireOpenNat->current.enabled = m_requireOpenNat->reset.enabled;															

									bNonHost = true;
								}

								/*if (Settings[host_forcehost].Value.bValue)
								{
									static bool flag = false;
									if (!*(int*)(Offsets::host) && !flag)
									{
										m_connectTimeOut->current.integer = 1;
										m_bandwithTestEnabled->current.enabled = false;
										m_autoTeams->current.enabled = true;
										m_maxTeamDifference->current.integer = 18;
										m_requireOpenNat->current.enabled = false;
										*m_doWeSuckAsHost = false;
										*m_natType = NAT_OPEN;										

										Engine.SendToConsole("xblive_privatematch 1; xpartygo;\n");

										flag = true;
									}
									else if (flag && *(int*)(Offsets::host))
									{
										Engine.SendToConsole("xblive_privatematch 0;\n");

										m_connectTimeOut->current.integer = m_connectTimeOut->reset.integer;
										m_bandwithTestEnabled->current.enabled = m_bandwithTestEnabled->reset.enabled;
										m_autoTeams->current.enabled = m_autoTeams->reset.enabled;
										m_maxTeamDifference->current.integer = m_maxTeamDifference->reset.integer;
										m_requireOpenNat->current.enabled = m_requireOpenNat->reset.enabled;

										flag = false;
									}
								}
								*/





							}

							if (!Settings[host_forcehost].Value.bValue)
							{

								ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
								ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							}

							{
								int XmodSize = xSize + 40 - 390;

								/*ImGui::SameLine(XmodSize - 69);
								ImGui::Checkbox("God mode", &Settings[host_god].Value.bValue);

								ImGui::SameLine(xSize - 270);*/

// 								ImGui::Checkbox("Auto wall", &Settings[host_autowall].Value.bValue);
// 								ImGui::SameLine(XmodSize);
								ImGui::Checkbox("Anti EndGame", &Settings[host_antiendgame].Value.bValue);	

// 								ImGui::Spacing();
// 
// 								ImGui::Checkbox("Kill cam", &Settings[host_killcam].Value.bValue);
// 
// 								ImGui::SameLine(XmodSize);
// 								ImGui::PushItemWidth(90);
// 								ImGui::SliderInt("Party Size", &Settings[host_partysize].Value.iValue, 2, 18);
// 								ImGui::PopItemWidth();									
							}

							if (!Settings[host_forcehost].Value.bValue)
							{
								ImGui::PopItemFlag();
								ImGui::PopStyleVar();
							}

						}
						ImGui::EndChild();
						break;

					case 5:
						ImGui::BeginChild("Keys Child", ImVec2(0, 295), true);
						{
							//keysize = ImGui::GetWindowSize();

							ImGui::Text("Numpad 1: Force class 1"); ImGui::SameLine(200); ImGui::Text("F1: Force class");
							ImGui::Text("Numpad 2: Force class 2"); ImGui::SameLine(200); ImGui::Text("F2: Switch team");
							ImGui::Text("Numpad 3: Force class 3"); ImGui::SameLine(200); ImGui::Text("F3: Go spectator");
							ImGui::Text("Numpad 4: Force class 4"); ImGui::SameLine(200); ImGui::Text("END: End game");
							ImGui::Text("Numpad 5: Force class 5");
							ImGui::Text("Numpad 6: Force class 6");
							ImGui::Text("Numpad 7: Force class 7");
							ImGui::Text("Numpad 8: Force class 8");
							ImGui::Text("Numpad 9: Force class 9");
							ImGui::Text("Numpad 0: Force class 10");							
						}
						ImGui::EndChild();
						break;

					case 6:
						ImGui::BeginChild("Style Child", ImVec2(0, 295), true);
						{
							SetStyle();
						}
						ImGui::EndChild();
						break;
					}



					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::Columns(4, nullptr, false);
					ImGui::NextColumn();

					if (ImGui::Button("Load", ImVec2(90, 30)))
					{
						if (Bot_LoadSettings() && LoadWindowsSettings())
						{
							ImGui::OpenPopup("Settings loaded");
						}
						else
						{
							ImGui::OpenPopup("Loading failed");
						}
					}
					ImGui::NextColumn();
					if (ImGui::Button("Save", ImVec2(90, 30)))
					{
						if (SaveWindowsSettings() && Bot_SaveSettings())
						{
							ImGui::OpenPopup("Settings saved");
						}
						else
						{
							ImGui::OpenPopup("Saving failed");
						}
					}

					if (ImGui::BeginPopupModal("Settings loaded", 0, ImGuiWindowFlags_NoResize))
					{
						ImGui::Text("The settings have been loaded");
						ImGui::Separator();
						if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}

					if (ImGui::BeginPopupModal("Settings saved", 0, ImGuiWindowFlags_NoResize))
					{
						ImGui::Text("The settings have been saved");
						ImGui::Separator();
						if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}

					if (ImGui::BeginPopupModal("Loading failed", 0, ImGuiWindowFlags_NoResize))
					{
						ImGui::Text("Failed to load the settings");
						ImGui::Separator();
						if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}

					if (ImGui::BeginPopupModal("Saving failed", 0, ImGuiWindowFlags_NoResize))
					{
						ImGui::Text("Failed to save the settings");
						ImGui::Separator();
						if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}



				}
			}
			ImGui::End();

		}
		else {
			if (!menu_flag)
				menu_flag = 1;
			if (menu_turn_off) {
				SafeCreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Hook.playSound), sounds.menu_toggle, 0, nullptr);
				menu_turn_off = 0;
			}
		}
	}

	void DrawInfo()
	{
		if (Settings[show_info].Value.bValue)
		{
			// (Load Saved Window Settings)....
			static bool bFirstTime4ShowInfo = false;
			if (!bFirstTime4ShowInfo)
			{				
				LoadWindowsSettings();
				bFirstTime4ShowInfo = true;
			}
			
			ImGui::SetNextWindowPos(Settings[window_info_pos].Value.v2Value, ImGuiSetCond_Appearing);

			ImGui::Begin("Info", &Settings[show_info].Value.bValue, Settings[window_info_size].Value.v2Value, -1.0f, ImGuiWindowFlags_NoCollapse);
			{
				//infoSize = ImGui::GetWindowSize();

				if (cg)
					ImGui::Text("Client Num: %i", cg->clientNum);

				int ping = *reinterpret_cast<int *>(Offsets::ping);
				int host = *reinterpret_cast<int*>(Offsets::host);

				ImGui::Text("Ping: %i", ping);

				if (host)
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "I'm HOST");
				else
					ImGui::Text("Non HOST");

				char *pName = reinterpret_cast<char*>(Offsets::namegame);
				if (pName)
					ImGui::Text("Name: %s", pName);

				if (cg_entities != nullptr)
				{
					weapon_t *pMYWEAPON = Engine.GetCombatWeapon(cg_entities[cg->clientNum].weaponID);
					if (pMYWEAPON)
					{
						if (pMYWEAPON->szLocalizedName/*szName*/ && !Aim.isVehicle)
							ImGui::Text("Weapon: %s", pMYWEAPON->szLocalizedName + 7/*szName*/);
						else if (pMYWEAPON->szName)
						{
							ImGui::Text("Weapon: %s", pMYWEAPON->szName);
						}

						/*ImGui::Text("1-iWeaponBulletType: %i", pMYWEAPON->iWeaponBulletType);
						ImGui::Text("2-bulletType: %i", pMYWEAPON->bulletType);
						*/
					}

				}

				if (perk_bulletPenetrationMultiplier)
					ImGui::Text("Bpm: %f", perk_bulletPenetrationMultiplier->getFloatValue());

				//========================================================================

				if (pStats)
				{
					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::Text("killstreak: %i", pStats->killstreak);
					ImGui::Text("HeadShots: %i", pStats->headshots);
					ImGui::Text("Deaths: %i", pStats->deaths);
					ImGui::Text("Xp: %i", pStats->xp);

					//check
					//ImGui::Text("K/D ratio: %i", pStats->kdratio);

					//float myAccuracy = pStats->accuracy / 100.0f;
					//ImGui::Text("Accuracy: %.2f%%", myAccuracy);

					ImGui::Text("Score: %i", pStats->score);
				}
			}
			ImGui::End();
		}
	}

	void DrawConsole()
	{
		if (g_bMainWindowOpen && Settings[show_console].Value.bValue)
			ShowConsole(&Settings[show_console].Value.bValue);
	}
	
	void DrawBackgroundImage(IDirect3DDevice9* pDevice)
	{			
		static std::string texfile;
		texfile = texturesFolder;
		texfile += "texture-2.jpg";		

		static bool bSprite = false;
		if (!bSprite)
		{
			pD3DSprite = new cSprite(mpos.x, mpos.y);
			if (pD3DSprite)
			{
				pD3DSprite->Init(pDevice, texfile.c_str(), msize.x, msize.y);
				bSprite = true;
			}
		}

		if (pD3DSprite && pD3DSprite->IsInit())
		{
			// Resizing (slow, but it works)
			// Todo: implement a better way

			static ImVec2 msizetemp = msize;			

			// both
			if (msizetemp.x != msize.x && msizetemp.y != msize.y)
			{				
				pD3DSprite->Release();
				delete pD3DSprite;
				pD3DSprite = new cSprite(mpos.x, mpos.y);
				if (pD3DSprite)
				{
					pD3DSprite->Init(pDevice, texfile.c_str(), msize.x, msize.y);
					msizetemp.x = msize.x;
					msizetemp.y = msize.y;					
				}
			}
			// only x
			else if (msizetemp.x != msize.x)
			{
				pD3DSprite->Release();
				delete pD3DSprite;
				pD3DSprite = new cSprite(mpos.x, mpos.y);
				if (pD3DSprite)
				{
					pD3DSprite->Init(pDevice, texfile.c_str(), msize.x, msize.y);
					msizetemp.x = msize.x;					
				}
			}
			// only y
			else if (msizetemp.y != msize.y)
			{
				pD3DSprite->Release();
				delete pD3DSprite;
				pD3DSprite = new cSprite(mpos.x, mpos.y);
				if (pD3DSprite)
				{
					pD3DSprite->Init(pDevice, texfile.c_str(), msize.x, msize.y);
					msizetemp.y = msize.y;					
				}
			}			
			

			if (pD3DSprite && pD3DSprite->IsInit() && g_bMainWindowOpen)
			{
				pD3DSprite->pPos.x = mpos.x;
				pD3DSprite->pPos.y = mpos.y;
				pD3DSprite->Draw();
			}
		}
	}


	/************************************************************************/
	/* D3D Hooks                                                            */
	/************************************************************************/

	EndScene_t OrigEndScene = nullptr;
	HRESULT __stdcall EndSceneHook(LPDIRECT3DDEVICE9 pDevice)
	{
		if (!g_bWasInitialized)
		{
			GUI_Init(pDevice);
		}
		else
		{
			static float y = 0;
			if (y > 239)
				y = 0;
			ImGui::ColorConvertHSVtoRGB(y / 239, 0.6, 0.5, colRainBow.x, colRainBow.y, colRainBow.z);
			y += 0.5f;

			if (Settings[d3d_enabled].Value.bValue)
			{
				if (Settings[d3d_chams].Value.bValue)
				{
					if (Settings[d3d_chams_axis].Value.bValue)
					{
						SetTextureColor(texAxis, D3DCOLOR_ARGB(
							static_cast<unsigned int>(Settings[d3d_axis_color].Value.v4Value.w * 255.0f),
							static_cast<unsigned int>(Settings[d3d_axis_color].Value.v4Value.x * 255.0f),
							static_cast<unsigned int>(Settings[d3d_axis_color].Value.v4Value.y * 255.0f),
							static_cast<unsigned int>(Settings[d3d_axis_color].Value.v4Value.z * 255.0f)));

						SetTextureColor(texAxisVisible, D3DCOLOR_ARGB(
							static_cast<unsigned int>(Settings[d3d_axis_visible_color].Value.v4Value.w * 255.0f),
							static_cast<unsigned int>(Settings[d3d_axis_visible_color].Value.v4Value.x * 255.0f),
							static_cast<unsigned int>(Settings[d3d_axis_visible_color].Value.v4Value.y * 255.0f),
							static_cast<unsigned int>(Settings[d3d_axis_visible_color].Value.v4Value.z * 255.0f)));
					}

					if (Settings[d3d_chams_allies].Value.bValue)
					{
						SetTextureColor(texAllies, D3DCOLOR_ARGB(
							static_cast<unsigned int>(Settings[d3d_allies_color].Value.v4Value.w * 255.0f),
							static_cast<unsigned int>(Settings[d3d_allies_color].Value.v4Value.x * 255.0f),
							static_cast<unsigned int>(Settings[d3d_allies_color].Value.v4Value.y * 255.0f),
							static_cast<unsigned int>(Settings[d3d_allies_color].Value.v4Value.z * 255.0f)));

						SetTextureColor(texAlliesVisible, D3DCOLOR_ARGB(
							static_cast<unsigned int>(Settings[d3d_allies_visible_color].Value.v4Value.w * 255.0f),
							static_cast<unsigned int>(Settings[d3d_allies_visible_color].Value.v4Value.x * 255.0f),
							static_cast<unsigned int>(Settings[d3d_allies_visible_color].Value.v4Value.y * 255.0f),
							static_cast<unsigned int>(Settings[d3d_allies_visible_color].Value.v4Value.z * 255.0f)));
					}

					if (Settings[d3d_chams_weapons].Value.bValue && !Settings[d3d_weapon_customtexture].Value.bValue)
					{
						SetTextureColor(texWeapon, D3DCOLOR_ARGB(
							static_cast<unsigned int>(Settings[d3d_weapon_color].Value.v4Value.w * 255.0f),
							static_cast<unsigned int>(Settings[d3d_weapon_color].Value.v4Value.x * 255.0f),
							static_cast<unsigned int>(Settings[d3d_weapon_color].Value.v4Value.y * 255.0f),
							static_cast<unsigned int>(Settings[d3d_weapon_color].Value.v4Value.z * 255.0f)));

						SetTextureColor(texWeaponVisible, D3DCOLOR_ARGB(
							static_cast<unsigned int>(Settings[d3d_weapon_visible_color].Value.v4Value.w * 255.0f),
							static_cast<unsigned int>(Settings[d3d_weapon_visible_color].Value.v4Value.x * 255.0f),
							static_cast<unsigned int>(Settings[d3d_weapon_visible_color].Value.v4Value.y * 255.0f),
							static_cast<unsigned int>(Settings[d3d_weapon_visible_color].Value.v4Value.z * 255.0f)));
					}
				}
			}

			auto& io = ImGui::GetIO();
			io.MouseDrawCursor = g_bMainWindowOpen;

			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			{
				DrawRadar();
				DrawMenu();
				DrawInfo();	
				DrawConsole();
				DrawBackgroundImage(pDevice);
			}

			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());


			

		}

		return OrigEndScene(pDevice);
	}

	Reset_t OrigReset = nullptr;
	HRESULT __stdcall ResetHook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		//GUI wasn't initialized yet, just call Reset and return
		if (!g_bWasInitialized)
			return OrigReset(pDevice, pPresentationParameters);

		ImGui_ImplDX9_InvalidateDeviceObjects(); //Invalidate GUI resources

		HRESULT hr = OrigReset(pDevice, pPresentationParameters);

		ImGui_ImplDX9_CreateDeviceObjects(); //Recreate GUI resources		

		return hr;

	}

	//========================================================================

	DrawIndexedPrimitive_t OrigDrawIndexedPrimitive = nullptr;	
	DWORD model = 0; DWORD weapon = 0; DWORD smoke = 0; DWORD specialval = 0;
	
	bool bTeamAxis(char *pModel)
	{
		if (strstr(pModel, "arab")
			|| strstr(pModel, "opforce")
			|| strstr(pModel, "russian")
			|| strstr(pModel, "airborne")
			|| strstr(pModel, "arctic")
			|| strstr(pModel, "op_arctic")
			|| strstr(pModel, "militia")
			|| strstr(pModel, "slum")
			|| strstr(pModel, "_ghillie_")
			)
			return true;

		return false;

	}

	bool bTeamAllies(char *pModel)
	{
		if (strstr(pModel, "141")
			|| strstr(pModel, "_us_")
			|| strstr(pModel, "ally")
			|| strstr(pModel, "_seal_"))
			return true;

		return false;
	}

	bool bWeapons(char *pModel)
	{
		if (strstr(pModel, "mc/mtl") && (strstr(pModel, "weapon") ||
			strstr(pModel, "betty")
			|| strstr(pModel, "pavelow")
			|| strstr(pModel, "mi24p")
			|| strstr(pModel, "cobra_low")
			|| strstr(pModel, "cobra_window_low")
			|| strstr(pModel, "cobra_rotor_low")
			|| strstr(pModel, "ac130")
			|| strstr(pModel, "remote")
			|| strstr(pModel, "stealth")
			|| strstr(pModel, "airstrike")
			|| strstr(pModel, "uav")
			|| strstr(pModel, "emergency_flare"))
			)
			return true;

		return false;
	}

	bool bHands(char *pModel)
	{
		if (strstr(pModel, "viewmodel")
			|| strstr(pModel, "ghillie")
			|| strstr(pModel, "sleeve")
			|| strstr(pModel, "arm"))
			return true;

		return false;
	}

	HRESULT __stdcall DrawIndexedPrimitiveHook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
	{
		if (!g_bWasInitialized)
			return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

		if (_ReturnAddress() == (void *)0x004573B6) //R_DrawXModelSkinnedCached 
		{
			if (!Settings[d3d_enabled].Value.bValue)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (!Settings[d3d_chams].Value.bValue &&
				!Settings[d3d_chams_wireframe].Value.bValue &&
				!Settings[d3d_chams_axis].Value.bValue &&
				!Settings[d3d_chams_allies].Value.bValue)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (!model)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			char *pModel = **(char ***)(model + 0xB8);
			if (pModel == nullptr)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (Settings[d3d_nohands].Value.bValue && bHands(pModel))				
				return D3D_OK;

			/*if (Settings[d3d_tripmap].Value.bValue && texMap)
				pDevice->SetTexture(0, texMap);*/

			if (Settings[d3d_chams].Value.bValue)
			{
				if (Settings[d3d_chams_axis].Value.bValue && bTeamAxis(pModel))
				{
					if (texAxis)
						pDevice->SetTexture(0, texAxis);
					if (Settings[d3d_chams_wireframe].Value.bValue)
						pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
					pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
					OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
					pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
					if (texAxisVisible)
						pDevice->SetTexture(0, texAxisVisible);
				}
				else if (Settings[d3d_chams_allies].Value.bValue && bTeamAllies(pModel))
				{
					if (texAllies)
						pDevice->SetTexture(0, texAllies);
					if (Settings[d3d_chams_wireframe].Value.bValue)
						pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
					pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
					OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
					pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
					if (texAlliesVisible)
						pDevice->SetTexture(0, texAlliesVisible);
				}
				else if (Settings[d3d_chams_wireframe].Value.bValue)
					pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
			else if (Settings[d3d_chams_wireframe].Value.bValue)
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		else if (_ReturnAddress() == (void *)/*the function: 0x0045C3B0*/0x0045C9D6) //weapons 0045C9D6: the retaddy
		{
			if (!Settings[d3d_enabled].Value.bValue)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (!Settings[d3d_chams].Value.bValue &&
				!Settings[d3d_chams_wireframe].Value.bValue &&
				!Settings[d3d_chams_weapons].Value.bValue)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (!weapon)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			char *pModel = **(char ***)(weapon + 0xB8);
			if (pModel == nullptr)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (Settings[d3d_noweapon].Value.bValue)
			{
				if (strstr(pModel, "mtl_weapon_") ||
					strstr(pModel, "emergency_flare"))
				{
					return D3D_OK;
				}
			}

			/*if (Settings[d3d_tripmap].Value.bValue && texMap)
				pDevice->SetTexture(0, texMap);*/

			if (Settings[d3d_chams].Value.bValue && Settings[d3d_chams_weapons].Value.bValue)
			{
				if (bWeapons(pModel))
				{
					pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

					//========================================================================
					if (Settings[d3d_weapon_customtexture].Value.bValue && texWeaponCustom)
						pDevice->SetTexture(0, texWeaponCustom);
					else if (texWeapon)
						pDevice->SetTexture(0, texWeapon);
					//========================================================================

					if (Settings[d3d_chams_wireframe].Value.bValue)
						pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

					OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

					pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

					//========================================================================
					if (Settings[d3d_weapon_customtexture].Value.bValue && texWeaponCustom)
						pDevice->SetTexture(0, texWeaponCustom);
					else if (texWeaponVisible)
						pDevice->SetTexture(0, texWeaponVisible);
					//========================================================================

				}
				else if (Settings[d3d_chams_wireframe].Value.bValue)
					pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
			else if (Settings[d3d_chams_wireframe].Value.bValue)
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		else if (_ReturnAddress() == (void *)/*the function: sub_458890*/0x00458AED)
		{
			if (!Settings[d3d_enabled].Value.bValue)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (!weapon)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			char *pModel = **(char ***)(weapon + 0xB8);
			if (pModel == nullptr)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			//here.
			if (Settings[d3d_chams_wireframe].Value.bValue)
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			if (Settings[d3d_nosky].Value.bValue)
			{
				if (strstr(pModel, "wc/sky"))
				{
					return D3D_OK;
				}
			}

			/*if (Settings[d3d_tripmap].Value.bValue && texMap)
				pDevice->SetTexture(0, texMap);*/

			if (Settings[d3d_glass].Value.bValue)
			{
				//wc/ch_concretewall
				//wc/me_cinderblock_wall
				//wc/me_cargocontainerwhite_ext_wall
				//wc/ch_wall
				//wc/me_cargocontainer_exteriorwall

				if (strstr(pModel, "wall")) {
					pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
					pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
				}
			}
		}
		else if (_ReturnAddress() == (void *)/*the function: sub_456290*/0x00456469)
		{
			if (!Settings[d3d_enabled].Value.bValue)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (!smoke)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			char *pModel = **(char ***)(smoke + 0xB8);
			if (pModel == nullptr)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			/*if (GetAsyncKeyState(VK_DELETE))
			{
				char pStrBuffer[128];
				sprintf_s(pStrBuffer, "-pModel: [%s]\n", pModel);
				console.AddLog(pStrBuffer);
			}*/

			if (Settings[d3d_nosmoke].Value.bValue)
			{
				if (strstr(pModel, "gfx_smk_") ||
					strstr(pModel, "_smoke_"))
				{
					return D3D_OK;
				}
			}
		}
		else if (_ReturnAddress() == (void *)/*the function: sub_456290*/0x45e2e0)
		{
			if (!Settings[d3d_enabled].Value.bValue)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			if (!specialval)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			char *pModel = **(char ***)(specialval + 0xB8);
			if (pModel == nullptr)
				return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

			/*if (GetAsyncKeyState(VK_DELETE))
			{
			char pStrBuffer[128];
			sprintf_s(pStrBuffer, "-pModel: [%s]\n", pModel);
			console.AddLog(pStrBuffer);
			}*/

			/*if (Settings[d3d_tripmap].Value.bValue && texMap)
				pDevice->SetTexture(0, texMap);	*/
		}
		else if (Settings[d3d_chams_wireframe].Value.bValue)
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
	}
	

	HRESULT __stdcall DrawIndexedPrimitive_Stub(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
	{
		/*
		Crash...
		if (!g_bWasInitialized)
		return OrigDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);*/

		__asm 
		{
			push eax

			mov eax, [esp + 0x40]; //0x3C(real) + 4 because of this push eax
			mov model, eax
			mov eax, [esp + 0x28] //looking for the struct in dip, before retaddress for weapons, yep, 0x24 + 4 cuz push eax., it works for sky/walls too.
			mov weapon, eax
			mov eax, [esp + 0x7C]; //0x78 + 4 cuz push eax
			mov smoke, eax
			mov eax, [esp + 0x24]
			mov specialval, eax
				
			pop eax

			jmp DrawIndexedPrimitiveHook
		}

		//It won't work like this & commented jmp instruction: return DrawIndexedPrimitiveHook(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

		//It doesn't draw & commented jmp instruction-> (without return) DrawIndexedPrimitiveHook(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);

	}

#define MAX_VIRTUALKEYS 0xFF // 256 virtual key codes (multibyte)

	struct sKeyMessages
	{
		bool bKey;
		bool bDown;
		bool bUp;
	} KeyMessages[MAX_VIRTUALKEYS]; // keypress structure

	bool GetKeyMessages(sKeyMessages* km, bool wait_for_release = 0)
	{
		if (km->bKey)
		{
			km->bUp = false;
			km->bDown = true;
		}

		else if (!km->bKey && km->bDown)
		{
			km->bUp = true;
			km->bDown = false;
		}

		else
		{
			km->bUp = false;
			km->bDown = false;
		}

		if (wait_for_release)
			return km->bUp;

		else
			return km->bDown;
	}

	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			KeyMessages[VK_LBUTTON].bKey = true;
			break;

		case WM_LBUTTONUP:
			KeyMessages[VK_LBUTTON].bKey = false;
			break;

		case WM_RBUTTONDOWN:
			KeyMessages[VK_RBUTTON].bKey = true;
			break;

		case WM_RBUTTONUP:
			KeyMessages[VK_RBUTTON].bKey = false;
			break;

		case WM_MBUTTONDOWN:
			KeyMessages[VK_MBUTTON].bKey = true;
			break;

		case WM_MBUTTONUP:
			KeyMessages[VK_MBUTTON].bKey = false;
			break;

		case WM_KEYDOWN:
			KeyMessages[wParam].bKey = true;
			break;

		case WM_KEYUP:
			KeyMessages[wParam].bKey = false;
			break;
		}

		//Aimbot
		Aim.g_bAimbotKey[Aim_t::g_bAimbotL] = GetKeyMessages(&KeyMessages[VK_LBUTTON]) ? true : false;
		Aim.g_bAimbotKey[Aim_t::g_bAimbotR] = GetKeyMessages(&KeyMessages[VK_RBUTTON]) ? true : false;
		Aim.g_bAimbotKey[Aim_t::g_bAimbotM] = GetKeyMessages(&KeyMessages[VK_MBUTTON]) ? true : false;
		Aim.g_bAimbotKey[Aim_t::g_bAimbotCtrl] = GetKeyMessages(&KeyMessages[VK_CONTROL]) ? true : false;
		Aim.g_bAimbotKey[Aim_t::g_bAimbotShift] = GetKeyMessages(&KeyMessages[VK_SHIFT]) ? true : false;

		//Menu (Spam Fix)
		/*static bool bMenu = false;
		if (!bMenu && GetKeyMessages(&KeyMessages[VK_INSERT]))
		{
			g_bMainWindowOpen = !g_bMainWindowOpen;
			bMenu = true;
		}
		else if (bMenu && !GetKeyMessages(&KeyMessages[VK_INSERT]))
		{
			bMenu = false;
		}*/


		if (GetKeyMessages(&KeyMessages[VK_INSERT], true))
			g_bMainWindowOpen = !g_bMainWindowOpen;


		//Classes
		g_bF1 = GetKeyMessages(&KeyMessages[VK_F1]) ? true : false;
		g_bF2 = GetKeyMessages(&KeyMessages[VK_F2]) ? true : false;
		g_bF3 = GetKeyMessages(&KeyMessages[VK_F3]) ? true : false;
		g_bF4 = GetKeyMessages(&KeyMessages[VK_F4]) ? true : false;
		g_bF5 = GetKeyMessages(&KeyMessages[VK_F5]) ? true : false;
		g_bF6 = GetKeyMessages(&KeyMessages[VK_F6]) ? true : false;
		g_bF7 = GetKeyMessages(&KeyMessages[VK_F7]) ? true : false;
		g_bF8 = GetKeyMessages(&KeyMessages[VK_F8]) ? true : false;
		g_bF9 = GetKeyMessages(&KeyMessages[VK_F9]) ? true : false;
		g_bF10 = GetKeyMessages(&KeyMessages[VK_F10]) ? true : false;
		g_bEND = GetKeyMessages(&KeyMessages[VK_END]) ? true : false;
		g_bNumPad1 = GetKeyMessages(&KeyMessages[VK_NUMPAD1]) ? true : false;
		g_bNumPad2 = GetKeyMessages(&KeyMessages[VK_NUMPAD2]) ? true : false;
		g_bNumPad3 = GetKeyMessages(&KeyMessages[VK_NUMPAD3]) ? true : false;
		g_bNumPad4 = GetKeyMessages(&KeyMessages[VK_NUMPAD4]) ? true : false;
		g_bNumPad5 = GetKeyMessages(&KeyMessages[VK_NUMPAD5]) ? true : false;
		g_bNumPad6 = GetKeyMessages(&KeyMessages[VK_NUMPAD6]) ? true : false;
		g_bNumPad7 = GetKeyMessages(&KeyMessages[VK_NUMPAD7]) ? true : false;
		g_bNumPad8 = GetKeyMessages(&KeyMessages[VK_NUMPAD8]) ? true : false;
		g_bNumPad9 = GetKeyMessages(&KeyMessages[VK_NUMPAD9]) ? true : false;
		g_bNumPad0 = GetKeyMessages(&KeyMessages[VK_NUMPAD0]) ? true : false;

		if (g_bWasInitialized && g_bMainWindowOpen && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(g_pOldWindowProc, hWnd, uMsg, wParam, lParam);
	}


	/************************************************************************/
	/* Init and Hook D3D                                                    */
	/************************************************************************/

	bool InitAndHookD3D()
	{
		/*

		006561BF   A1 28CCFB05      MOV EAX,DWORD PTR DS:[0x5FBCC28] <<<<<
		006561C4   8B08             MOV ECX,DWORD PTR DS:[EAX]
		006561C6   56               PUSH ESI
		006561C7   8B7424 0C        MOV ESI,DWORD PTR SS:[ESP+0xC]
		006561CB   6A 00            PUSH 0x0
		006561CD   52               PUSH EDX
		006561CE   6A 00            PUSH 0x0
		006561D0   6A 00            PUSH 0x0
		006561D2   68 08020000      PUSH 0x208
		006561D7   56               PUSH ESI
		006561D8   50               PUSH EAX
		006561D9   8B41 68          MOV EAX,DWORD PTR DS:[ECX+0x68]
		006561DC   FFD0             CALL EAX
		006561DE   85C0             TEST EAX,EAX
		006561E0   7D 1B            JGE SHORT iw5mp.006561FD
		006561E2   50               PUSH EAX
		006561E3   E8 A8740100      CALL iw5mp.0066D690
		006561E8   50               PUSH EAX
		006561E9   56               PUSH ESI
		006561EA   68 58398200      PUSH iw5mp.00823958                      ; ASCII "DirectX didn't create a %i-byte dynamic vertex buffer: %s
		"
		iw5mp Base 400000 Size 68a5000 \xA1\x00\x00\x00\x00\x8B\x08\x56\x8B\x74\x24\x0C\x6A\x00\x52\x6A\x00\x6A\x00\x68\x00\x00\x00\x00\x56\x50\x8B\x41\x68\xFF\xD0\x85\xC0\x7D\x1B\x50\xE8\x00\x00\x00\x00\x50\x56\x68\x00\x00\x00\x00 x????xxxxxxxxxxxxxxx????xxxxxxxxxxxxx????xxx????


		0066F3F9   A1 28CCFB05      MOV EAX,DWORD PTR DS:[0x5FBCC28] <<<<<
		0066F3FE   8B08             MOV ECX,DWORD PTR DS:[EAX]
		0066F400   83C4 10          ADD ESP,0x10
		0066F403   8BD6             MOV EDX,ESI
		0066F405   52               PUSH EDX
		0066F406   50               PUSH EAX
		0066F407   8B41 40          MOV EAX,DWORD PTR DS:[ECX+0x40]
		0066F40A   FFD0             CALL EAX
		0066F40C   8BF0             MOV ESI,EAX
		0066F40E   85F6             TEST ESI,ESI
		0066F410   7D 1A            JGE SHORT iw5mp.0066F42C
		0066F412   56               PUSH ESI
		0066F413   E8 9EB00500      CALL iw5mp.006CA4B6
		0066F418   50               PUSH EAX
		0066F419   56               PUSH ESI
		0066F41A   68 40558300      PUSH iw5mp.00835540                      ; ASCII "Couldn't reset a lost Direct3D device - IDirect3DDevice9::Reset returned 0x%08x (%s)"

		0067FDED   A1 28CCFB05      MOV EAX,DWORD PTR DS:[0x5FBCC28] <<<<<<
		0067FDF2   8B08             MOV ECX,DWORD PTR DS:[EAX]
		0067FDF4   56               PUSH ESI
		0067FDF5   8B35 8020FC05    MOV ESI,DWORD PTR DS:[0x5FC2080]
		0067FDFB   57               PUSH EDI
		0067FDFC   8B3D 8420FC05    MOV EDI,DWORD PTR DS:[0x5FC2084]
		0067FE02   6A 00            PUSH 0x0
		0067FE04   68 E8F8FB05      PUSH iw5mp.05FBF8E8
		0067FE09   6A 00            PUSH 0x0
		0067FE0B   6A 00            PUSH 0x0
		0067FE0D   6A 00            PUSH 0x0
		0067FE0F   52               PUSH EDX
		0067FE10   57               PUSH EDI
		0067FE11   56               PUSH ESI
		0067FE12   50               PUSH EAX
		0067FE13   8B41 74          MOV EAX,DWORD PTR DS:[ECX+0x74]
		0067FE16   FFD0             CALL EAX
		0067FE18   85C0             TEST EAX,EAX
		0067FE1A   7D 18            JGE SHORT iw5mp.0067FE34
		0067FE1C   50               PUSH EAX
		0067FE1D   E8 6ED8FEFF      CALL iw5mp.0066D690
		0067FE22   50               PUSH EAX
		0067FE23   57               PUSH EDI
		0067FE24   56               PUSH ESI
		0067FE25   68 F86F8300      PUSH iw5mp.00836FF8                      ; ASCII "Couldn't create a %i x %i depth-stencil surface: %s
		"

		0067FEF2   A1 28CCFB05      MOV EAX,DWORD PTR DS:[0x5FBCC28] <<<
		0067FEF7   8B15 6C29FC05    MOV EDX,DWORD PTR DS:[0x5FC296C]
		0067FEFD   8B08             MOV ECX,DWORD PTR DS:[EAX]
		0067FEFF   83C4 0C          ADD ESP,0xC
		0067FF02   0FB7DB           MOVZX EBX,BX
		0067FF05   0FB7FF           MOVZX EDI,DI
		0067FF08   6A 00            PUSH 0x0
		0067FF0A   85ED             TEST EBP,EBP
		0067FF0C   75 30            JNZ SHORT iw5mp.0067FF3E
		0067FF0E   83C6 04          ADD ESI,0x4
		0067FF11   56               PUSH ESI
		0067FF12   55               PUSH EBP
		0067FF13   55               PUSH EBP
		0067FF14   55               PUSH EBP
		0067FF15   52               PUSH EDX
		0067FF16   53               PUSH EBX
		0067FF17   57               PUSH EDI
		0067FF18   50               PUSH EAX
		0067FF19   8B41 70          MOV EAX,DWORD PTR DS:[ECX+0x70]
		0067FF1C   FFD0             CALL EAX
		0067FF1E   85C0             TEST EAX,EAX
		0067FF20   7D 4B            JGE SHORT iw5mp.0067FF6D
		0067FF22   50               PUSH EAX
		0067FF23   E8 68D7FEFF      CALL iw5mp.0066D690
		0067FF28   50               PUSH EAX
		0067FF29   53               PUSH EBX
		0067FF2A   57               PUSH EDI
		0067FF2B   68 30708300      PUSH iw5mp.00837030                      ; ASCII "Couldn't create a %i x %i render target surface: %s
		"

		*/

		PDWORD* pVTable = nullptr;
		for (; pVTable == nullptr; SafeSleep(217))
			pVTable = *reinterpret_cast<PDWORD **>(/*0x5FBCC28*/ 0x6664C60);		

		//Because: for vac banned users, 
		//is possible to play after injecting that public bypass module that edit the window's game
		while (!(g_hWindow = FindWindowA("IW4", 0)))
			SafeSleep(/*217*/218);		

		g_pOldWindowProc =
			reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooked_WndProc)));

		pD3DHook = new CVMTHookManager;
		if (g_pOldWindowProc && pD3DHook && pD3DHook->bInitialize(pVTable))
		{
			OrigDrawIndexedPrimitive = 
				reinterpret_cast<DrawIndexedPrimitive_t>(pD3DHook->dwHookMethod(reinterpret_cast<DWORD>(DrawIndexedPrimitive_Stub), INDEX_DIP));

			OrigEndScene = 
				reinterpret_cast<EndScene_t>(pD3DHook->dwHookMethod(reinterpret_cast<DWORD>(EndSceneHook), INDEX_ENDSCENE));

			OrigReset = 
				reinterpret_cast<Reset_t>(pD3DHook->dwHookMethod(reinterpret_cast<DWORD>(ResetHook), INDEX_RESET));

			if (OrigDrawIndexedPrimitive &&
				OrigEndScene &&
				OrigReset)
			{				
				return true;
			}
			else
			{				
				pD3DHook->UnHook();
				pD3DHook = nullptr;
				SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_pOldWindowProc));
				
				return false;
			}
		}
		else
		{
			return false;
		}
	}

}

/*TheMysteryPanda03/29/2018
swprintf(PathBuffer, 512, L"%s\\RedTex.png", DllPath);
D3DXCreateTextureFromFileEx(Device, PathBuffer, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, NULL, NULL, &RedTexture);
call it in your DrawIndexedPrimitiveHook*/