#include "stdafx.h"
#include <string.h>
#include "blowfish.h"

#define BLOWFISH_NUMBER               16

static const unsigned long BLOWFISH_ORIG_P[16 + 2] = {
        0x243F6A88L, 0x85A308D3L, 0x13198A2EL, 0x03707344L,
        0xA4093822L, 0x299F31D0L, 0x082EFA98L, 0xEC4E6C89L,
        0x452821E6L, 0x38D01377L, 0xBE5466CFL, 0x34E90C6CL,
        0xC0AC29B7L, 0xC97C50DDL, 0x3F84D5B5L, 0xB5470917L,
        0x9216D5D9L, 0x8979FB1BL
};

static const unsigned long BLOWFISH_ORIG_S[4][256] = {
    {   0xD1310BA6L, 0x98DFB5ACL, 0x2FFD72DBL, 0xD01ADFB7L,
        0xB8E1AFEDL, 0x6A267E96L, 0xBA7C9045L, 0xF12C7F99L,
        0x24A19947L, 0xB3916CF7L, 0x0801F2E2L, 0x858EFC16L,
        0x636920D8L, 0x71574E69L, 0xA458FEA3L, 0xF4933D7EL,
        0x0D95748FL, 0x728EB658L, 0x718BCD58L, 0x82154AEEL,
        0x7B54A41DL, 0xC25A59B5L, 0x9C30D539L, 0x2AF26013L,
        0xC5D1B023L, 0x286085F0L, 0xCA417918L, 0xB8DB38EFL,
        0x8E79DCB0L, 0x603A180EL, 0x6C9E0E8BL, 0xB01E8A3EL,
        0xD71577C1L, 0xBD314B27L, 0x78AF2FDAL, 0x55605C60L,
        0xE65525F3L, 0xAA55AB94L, 0x57489862L, 0x63E81440L,
        0x55CA396AL, 0x2AAB10B6L, 0xB4CC5C34L, 0x1141E8CEL,
        0xA15486AFL, 0x7C72E993L, 0xB3EE1411L, 0x636FBC2AL,
        0x2BA9C55DL, 0x741831F6L, 0xCE5C3E16L, 0x9B87931EL,
        0xAFD6BA33L, 0x6C24CF5CL, 0x7A325381L, 0x28958677L,
        0x3B8F4898L, 0x6B4BB9AFL, 0xC4BFE81BL, 0x66282193L,
        0x61D809CCL, 0xFB21A991L, 0x487CAC60L, 0x5DEC8032L,
        0xEF845D5DL, 0xE98575B1L, 0xDC262302L, 0xEB651B88L,
        0x23893E81L, 0xD396ACC5L, 0x0F6D6FF3L, 0x83F44239L,
        0x2E0B4482L, 0xA4842004L, 0x69C8F04AL, 0x9E1F9B5EL,
        0x21C66842L, 0xF6E96C9AL, 0x670C9C61L, 0xABD388F0L,
        0x6A51A0D2L, 0xD8542F68L, 0x960FA728L, 0xAB5133A3L,
        0x6EEF0B6CL, 0x137A3BE4L, 0xBA3BF050L, 0x7EFB2A98L,
        0xA1F1651DL, 0x39AF0176L, 0x66CA593EL, 0x82430E88L,
        0x8CEE8619L, 0x456F9FB4L, 0x7D84A5C3L, 0x3B8B5EBEL,
        0xE06F75D8L, 0x85C12073L, 0x401A449FL, 0x56C16AA6L,
        0x4ED3AA62L, 0x363F7706L, 0x1BFEDF72L, 0x429B023DL,
        0x37D0D724L, 0xD00A1248L, 0xDB0FEAD3L, 0x49F1C09BL,
        0x075372C9L, 0x80991B7BL, 0x25D479D8L, 0xF6E8DEF7L,
        0xE3FE501AL, 0xB6794C3BL, 0x976CE0BDL, 0x04C006BAL,
        0xC1A94FB6L, 0x409F60C4L, 0x5E5C9EC2L, 0x196A2463L,
        0x68FB6FAFL, 0x3E6C53B5L, 0x1339B2EBL, 0x3B52EC6FL,
        0x6DFC511FL, 0x9B30952CL, 0xCC814544L, 0xAF5EBD09L,
        0xBEE3D004L, 0xDE334AFDL, 0x660F2807L, 0x192E4BB3L,
        0xC0CBA857L, 0x45C8740FL, 0xD20B5F39L, 0xB9D3FBDBL,
        0x5579C0BDL, 0x1A60320AL, 0xD6A100C6L, 0x402C7279L,
        0x679F25FEL, 0xFB1FA3CCL, 0x8EA5E9F8L, 0xDB3222F8L,
        0x3C7516DFL, 0xFD616B15L, 0x2F501EC8L, 0xAD0552ABL,
        0x323DB5FAL, 0xFD238760L, 0x53317B48L, 0x3E00DF82L,
        0x9E5C57BBL, 0xCA6F8CA0L, 0x1A87562EL, 0xDF1769DBL,
        0xD542A8F6L, 0x287EFFC3L, 0xAC6732C6L, 0x8C4F5573L,
        0x695B27B0L, 0xBBCA58C8L, 0xE1FFA35DL, 0xB8F011A0L,
        0x10FA3D98L, 0xFD2183B8L, 0x4AFCB56CL, 0x2DD1D35BL,
        0x9A53E479L, 0xB6F84565L, 0xD28E49BCL, 0x4BFB9790L,
        0xE1DDF2DAL, 0xA4CB7E33L, 0x62FB1341L, 0xCEE4C6E8L,
        0xEF20CADAL, 0x36774C01L, 0xD07E9EFEL, 0x2BF11FB4L,
        0x95DBDA4DL, 0xAE909198L, 0xEAAD8E71L, 0x6B93D5A0L,
        0xD08ED1D0L, 0xAFC725E0L, 0x8E3C5B2FL, 0x8E7594B7L,
        0x8FF6E2FBL, 0xF2122B64L, 0x8888B812L, 0x900DF01CL,
        0x4FAD5EA0L, 0x688FC31CL, 0xD1CFF191L, 0xB3A8C1ADL,
        0x2F2F2218L, 0xBE0E1777L, 0xEA752DFEL, 0x8B021FA1L,
        0xE5A0CC0FL, 0xB56F74E8L, 0x18ACF3D6L, 0xCE89E299L,
        0xB4A84FE0L, 0xFD13E0B7L, 0x7CC43B81L, 0xD2ADA8D9L,
        0x165FA266L, 0x80957705L, 0x93CC7314L, 0x211A1477L,
        0xE6AD2065L, 0x77B5FA86L, 0xC75442F5L, 0xFB9D35CFL,
        0xEBCDAF0CL, 0x7B3E89A0L, 0xD6411BD3L, 0xAE1E7E49L,
        0x00250E2DL, 0x2071B35EL, 0x226800BBL, 0x57B8E0AFL,
        0x2464369BL, 0xF009B91EL, 0x5563911DL, 0x59DFA6AAL,
        0x78C14389L, 0xD95A537FL, 0x207D5BA2L, 0x02E5B9C5L,
        0x83260376L, 0x6295CFA9L, 0x11C81968L, 0x4E734A41L,
        0xB3472DCAL, 0x7B14A94AL, 0x1B510052L, 0x9A532915L,
        0xD60F573FL, 0xBC9BC6E4L, 0x2B60A476L, 0x81E67400L,
        0x08BA6FB5L, 0x571BE91FL, 0xF296EC6BL, 0x2A0DD915L,
        0xB6636521L, 0xE7B9F9B6L, 0xFF34052EL, 0xC5855664L,
        0x53B02D5DL, 0xA99F8FA1L, 0x08BA4799L, 0x6E85076AL   },
    {   0x4B7A70E9L, 0xB5B32944L, 0xDB75092EL, 0xC4192623L,
        0xAD6EA6B0L, 0x49A7DF7DL, 0x9CEE60B8L, 0x8FEDB266L,
        0xECAA8C71L, 0x699A17FFL, 0x5664526CL, 0xC2B19EE1L,
        0x193602A5L, 0x75094C29L, 0xA0591340L, 0xE4183A3EL,
        0x3F54989AL, 0x5B429D65L, 0x6B8FE4D6L, 0x99F73FD6L,
        0xA1D29C07L, 0xEFE830F5L, 0x4D2D38E6L, 0xF0255DC1L,
        0x4CDD2086L, 0x8470EB26L, 0x6382E9C6L, 0x021ECC5EL,
        0x09686B3FL, 0x3EBAEFC9L, 0x3C971814L, 0x6B6A70A1L,
        0x687F3584L, 0x52A0E286L, 0xB79C5305L, 0xAA500737L,
        0x3E07841CL, 0x7FDEAE5CL, 0x8E7D44ECL, 0x5716F2B8L,
        0xB03ADA37L, 0xF0500C0DL, 0xF01C1F04L, 0x0200B3FFL,
        0xAE0CF51AL, 0x3CB574B2L, 0x25837A58L, 0xDC0921BDL,
        0xD19113F9L, 0x7CA92FF6L, 0x94324773L, 0x22F54701L,
        0x3AE5E581L, 0x37C2DADCL, 0xC8B57634L, 0x9AF3DDA7L,
        0xA9446146L, 0x0FD0030EL, 0xECC8C73EL, 0xA4751E41L,
        0xE238CD99L, 0x3BEA0E2FL, 0x3280BBA1L, 0x183EB331L,
        0x4E548B38L, 0x4F6DB908L, 0x6F420D03L, 0xF60A04BFL,
        0x2CB81290L, 0x24977C79L, 0x5679B072L, 0xBCAF89AFL,
        0xDE9A771FL, 0xD9930810L, 0xB38BAE12L, 0xDCCF3F2EL,
        0x5512721FL, 0x2E6B7124L, 0x501ADDE6L, 0x9F84CD87L,
        0x7A584718L, 0x7408DA17L, 0xBC9F9ABCL, 0xE94B7D8CL,
        0xEC7AEC3AL, 0xDB851DFAL, 0x63094366L, 0xC464C3D2L,
        0xEF1C1847L, 0x3215D908L, 0xDD433B37L, 0x24C2BA16L,
        0x12A14D43L, 0x2A65C451L, 0x50940002L, 0x133AE4DDL,
        0x71DFF89EL, 0x10314E55L, 0x81AC77D6L, 0x5F11199BL,
        0x043556F1L, 0xD7A3C76BL, 0x3C11183BL, 0x5924A509L,
        0xF28FE6EDL, 0x97F1FBFAL, 0x9EBABF2CL, 0x1E153C6EL,
        0x86E34570L, 0xEAE96FB1L, 0x860E5E0AL, 0x5A3E2AB3L,
        0x771FE71CL, 0x4E3D06FAL, 0x2965DCB9L, 0x99E71D0FL,
        0x803E89D6L, 0x5266C825L, 0x2E4CC978L, 0x9C10B36AL,
        0xC6150EBAL, 0x94E2EA78L, 0xA5FC3C53L, 0x1E0A2DF4L,
        0xF2F74EA7L, 0x361D2B3DL, 0x1939260FL, 0x19C27960L,
        0x5223A708L, 0xF71312B6L, 0xEBADFE6EL, 0xEAC31F66L,
        0xE3BC4595L, 0xA67BC883L, 0xB17F37D1L, 0x018CFF28L,
        0xC332DDEFL, 0xBE6C5AA5L, 0x65582185L, 0x68AB9802L,
        0xEECEA50FL, 0xDB2F953BL, 0x2AEF7DADL, 0x5B6E2F84L,
        0x1521B628L, 0x29076170L, 0xECDD4775L, 0x619F1510L,
        0x13CCA830L, 0xEB61BD96L, 0x0334FE1EL, 0xAA0363CFL,
        0xB5735C90L, 0x4C70A239L, 0xD59E9E0BL, 0xCBAADE14L,
        0xEECC86BCL, 0x60622CA7L, 0x9CAB5CABL, 0xB2F3846EL,
        0x648B1EAFL, 0x19BDF0CAL, 0xA02369B9L, 0x655ABB50L,
        0x40685A32L, 0x3C2AB4B3L, 0x319EE9D5L, 0xC021B8F7L,
        0x9B540B19L, 0x875FA099L, 0x95F7997EL, 0x623D7DA8L,
        0xF837889AL, 0x97E32D77L, 0x11ED935FL, 0x16681281L,
        0x0E358829L, 0xC7E61FD6L, 0x96DEDFA1L, 0x7858BA99L,
        0x57F584A5L, 0x1B227263L, 0x9B83C3FFL, 0x1AC24696L,
        0xCDB30AEBL, 0x532E3054L, 0x8FD948E4L, 0x6DBC3128L,
        0x58EBF2EFL, 0x34C6FFEAL, 0xFE28ED61L, 0xEE7C3C73L,
        0x5D4A14D9L, 0xE864B7E3L, 0x42105D14L, 0x203E13E0L,
        0x45EEE2B6L, 0xA3AAABEAL, 0xDB6C4F15L, 0xFACB4FD0L,
        0xC742F442L, 0xEF6ABBB5L, 0x654F3B1DL, 0x41CD2105L,
        0xD81E799EL, 0x86854DC7L, 0xE44B476AL, 0x3D816250L,
        0xCF62A1F2L, 0x5B8D2646L, 0xFC8883A0L, 0xC1C7B6A3L,
        0x7F1524C3L, 0x69CB7492L, 0x47848A0BL, 0x5692B285L,
        0x095BBF00L, 0xAD19489DL, 0x1462B174L, 0x23820E00L,
        0x58428D2AL, 0x0C55F5EAL, 0x1DADF43EL, 0x233F7061L,
        0x3372F092L, 0x8D937E41L, 0xD65FECF1L, 0x6C223BDBL,
        0x7CDE3759L, 0xCBEE7460L, 0x4085F2A7L, 0xCE77326EL,
        0xA6078084L, 0x19F8509EL, 0xE8EFD855L, 0x61D99735L,
        0xA969A7AAL, 0xC50C06C2L, 0x5A04ABFCL, 0x800BCADCL,
        0x9E447A2EL, 0xC3453484L, 0xFDD56705L, 0x0E1E9EC9L,
        0xDB73DBD3L, 0x105588CDL, 0x675FDA79L, 0xE3674340L,
        0xC5C43465L, 0x713E38D8L, 0x3D28F89EL, 0xF16DFF20L,
        0x153E21E7L, 0x8FB03D4AL, 0xE6E39F2BL, 0xDB83ADF7L   },
    {   0xE93D5A68L, 0x948140F7L, 0xF64C261CL, 0x94692934L,
        0x411520F7L, 0x7602D4F7L, 0xBCF46B2EL, 0xD4A20068L,
        0xD4082471L, 0x3320F46AL, 0x43B7D4B7L, 0x500061AFL,
        0x1E39F62EL, 0x97244546L, 0x14214F74L, 0xBF8B8840L,
        0x4D95FC1DL, 0x96B591AFL, 0x70F4DDD3L, 0x66A02F45L,
        0xBFBC09ECL, 0x03BD9785L, 0x7FAC6DD0L, 0x31CB8504L,
        0x96EB27B3L, 0x55FD3941L, 0xDA2547E6L, 0xABCA0A9AL,
        0x28507825L, 0x530429F4L, 0x0A2C86DAL, 0xE9B66DFBL,
        0x68DC1462L, 0xD7486900L, 0x680EC0A4L, 0x27A18DEEL,
        0x4F3FFEA2L, 0xE887AD8CL, 0xB58CE006L, 0x7AF4D6B6L,
        0xAACE1E7CL, 0xD3375FECL, 0xCE78A399L, 0x406B2A42L,
        0x20FE9E35L, 0xD9F385B9L, 0xEE39D7ABL, 0x3B124E8BL,
        0x1DC9FAF7L, 0x4B6D1856L, 0x26A36631L, 0xEAE397B2L,
        0x3A6EFA74L, 0xDD5B4332L, 0x6841E7F7L, 0xCA7820FBL,
        0xFB0AF54EL, 0xD8FEB397L, 0x454056ACL, 0xBA489527L,
        0x55533A3AL, 0x20838D87L, 0xFE6BA9B7L, 0xD096954BL,
        0x55A867BCL, 0xA1159A58L, 0xCCA92963L, 0x99E1DB33L,
        0xA62A4A56L, 0x3F3125F9L, 0x5EF47E1CL, 0x9029317CL,
        0xFDF8E802L, 0x04272F70L, 0x80BB155CL, 0x05282CE3L,
        0x95C11548L, 0xE4C66D22L, 0x48C1133FL, 0xC70F86DCL,
        0x07F9C9EEL, 0x41041F0FL, 0x404779A4L, 0x5D886E17L,
        0x325F51EBL, 0xD59BC0D1L, 0xF2BCC18FL, 0x41113564L,
        0x257B7834L, 0x602A9C60L, 0xDFF8E8A3L, 0x1F636C1BL,
        0x0E12B4C2L, 0x02E1329EL, 0xAF664FD1L, 0xCAD18115L,
        0x6B2395E0L, 0x333E92E1L, 0x3B240B62L, 0xEEBEB922L,
        0x85B2A20EL, 0xE6BA0D99L, 0xDE720C8CL, 0x2DA2F728L,
        0xD0127845L, 0x95B794FDL, 0x647D0862L, 0xE7CCF5F0L,
        0x5449A36FL, 0x877D48FAL, 0xC39DFD27L, 0xF33E8D1EL,
        0x0A476341L, 0x992EFF74L, 0x3A6F6EABL, 0xF4F8FD37L,
        0xA812DC60L, 0xA1EBDDF8L, 0x991BE14CL, 0xDB6E6B0DL,
        0xC67B5510L, 0x6D672C37L, 0x2765D43BL, 0xDCD0E804L,
        0xF1290DC7L, 0xCC00FFA3L, 0xB5390F92L, 0x690FED0BL,
        0x667B9FFBL, 0xCEDB7D9CL, 0xA091CF0BL, 0xD9155EA3L,
        0xBB132F88L, 0x515BAD24L, 0x7B9479BFL, 0x763BD6EBL,
        0x37392EB3L, 0xCC115979L, 0x8026E297L, 0xF42E312DL,
        0x6842ADA7L, 0xC66A2B3BL, 0x12754CCCL, 0x782EF11CL,
        0x6A124237L, 0xB79251E7L, 0x06A1BBE6L, 0x4BFB6350L,
        0x1A6B1018L, 0x11CAEDFAL, 0x3D25BDD8L, 0xE2E1C3C9L,
        0x44421659L, 0x0A121386L, 0xD90CEC6EL, 0xD5ABEA2AL,
        0x64AF674EL, 0xDA86A85FL, 0xBEBFE988L, 0x64E4C3FEL,
        0x9DBC8057L, 0xF0F7C086L, 0x60787BF8L, 0x6003604DL,
        0xD1FD8346L, 0xF6381FB0L, 0x7745AE04L, 0xD736FCCCL,
        0x83426B33L, 0xF01EAB71L, 0xB0804187L, 0x3C005E5FL,
        0x77A057BEL, 0xBDE8AE24L, 0x55464299L, 0xBF582E61L,
        0x4E58F48FL, 0xF2DDFDA2L, 0xF474EF38L, 0x8789BDC2L,
        0x5366F9C3L, 0xC8B38E74L, 0xB475F255L, 0x46FCD9B9L,
        0x7AEB2661L, 0x8B1DDF84L, 0x846A0E79L, 0x915F95E2L,
        0x466E598EL, 0x20B45770L, 0x8CD55591L, 0xC902DE4CL,
        0xB90BACE1L, 0xBB8205D0L, 0x11A86248L, 0x7574A99EL,
        0xB77F19B6L, 0xE0A9DC09L, 0x662D09A1L, 0xC4324633L,
        0xE85A1F02L, 0x09F0BE8CL, 0x4A99A025L, 0x1D6EFE10L,
        0x1AB93D1DL, 0x0BA5A4DFL, 0xA186F20FL, 0x2868F169L,
        0xDCB7DA83L, 0x573906FEL, 0xA1E2CE9BL, 0x4FCD7F52L,
        0x50115E01L, 0xA70683FAL, 0xA002B5C4L, 0x0DE6D027L,
        0x9AF88C27L, 0x773F8641L, 0xC3604C06L, 0x61A806B5L,
        0xF0177A28L, 0xC0F586E0L, 0x006058AAL, 0x30DC7D62L,
        0x11E69ED7L, 0x2338EA63L, 0x53C2DD94L, 0xC2C21634L,
        0xBBCBEE56L, 0x90BCB6DEL, 0xEBFC7DA1L, 0xCE591D76L,
        0x6F05E409L, 0x4B7C0188L, 0x39720A3DL, 0x7C927C24L,
        0x86E3725FL, 0x724D9DB9L, 0x1AC15BB4L, 0xD39EB8FCL,
        0xED545578L, 0x08FCA5B5L, 0xD83D7CD3L, 0x4DAD0FC4L,
        0x1E50EF5EL, 0xB161E6F8L, 0xA28514D9L, 0x6C51133CL,
        0x6FD5C7E7L, 0x56E14EC4L, 0x362ABFCEL, 0xDDC6C837L,
        0xD79A3234L, 0x92638212L, 0x670EFA8EL, 0x406000E0L  },
    {   0x3A39CE37L, 0xD3FAF5CFL, 0xABC27737L, 0x5AC52D1BL,
        0x5CB0679EL, 0x4FA33742L, 0xD3822740L, 0x99BC9BBEL,
        0xD5118E9DL, 0xBF0F7315L, 0xD62D1C7EL, 0xC700C47BL,
        0xB78C1B6BL, 0x21A19045L, 0xB26EB1BEL, 0x6A366EB4L,
        0x5748AB2FL, 0xBC946E79L, 0xC6A376D2L, 0x6549C2C8L,
        0x530FF8EEL, 0x468DDE7DL, 0xD5730A1DL, 0x4CD04DC6L,
        0x2939BBDBL, 0xA9BA4650L, 0xAC9526E8L, 0xBE5EE304L,
        0xA1FAD5F0L, 0x6A2D519AL, 0x63EF8CE2L, 0x9A86EE22L,
        0xC089C2B8L, 0x43242EF6L, 0xA51E03AAL, 0x9CF2D0A4L,
        0x83C061BAL, 0x9BE96A4DL, 0x8FE51550L, 0xBA645BD6L,
        0x2826A2F9L, 0xA73A3AE1L, 0x4BA99586L, 0xEF5562E9L,
        0xC72FEFD3L, 0xF752F7DAL, 0x3F046F69L, 0x77FA0A59L,
        0x80E4A915L, 0x87B08601L, 0x9B09E6ADL, 0x3B3EE593L,
        0xE990FD5AL, 0x9E34D797L, 0x2CF0B7D9L, 0x022B8B51L,
        0x96D5AC3AL, 0x017DA67DL, 0xD1CF3ED6L, 0x7C7D2D28L,
        0x1F9F25CFL, 0xADF2B89BL, 0x5AD6B472L, 0x5A88F54CL,
        0xE029AC71L, 0xE019A5E6L, 0x47B0ACFDL, 0xED93FA9BL,
        0xE8D3C48DL, 0x283B57CCL, 0xF8D56629L, 0x79132E28L,
        0x785F0191L, 0xED756055L, 0xF7960E44L, 0xE3D35E8CL,
        0x15056DD4L, 0x88F46DBAL, 0x03A16125L, 0x0564F0BDL,
        0xC3EB9E15L, 0x3C9057A2L, 0x97271AECL, 0xA93A072AL,
        0x1B3F6D9BL, 0x1E6321F5L, 0xF59C66FBL, 0x26DCF319L,
        0x7533D928L, 0xB155FDF5L, 0x03563482L, 0x8ABA3CBBL,
        0x28517711L, 0xC20AD9F8L, 0xABCC5167L, 0xCCAD925FL,
        0x4DE81751L, 0x3830DC8EL, 0x379D5862L, 0x9320F991L,
        0xEA7A90C2L, 0xFB3E7BCEL, 0x5121CE64L, 0x774FBE32L,
        0xA8B6E37EL, 0xC3293D46L, 0x48DE5369L, 0x6413E680L,
        0xA2AE0810L, 0xDD6DB224L, 0x69852DFDL, 0x09072166L,
        0xB39A460AL, 0x6445C0DDL, 0x586CDECFL, 0x1C20C8AEL,
        0x5BBEF7DDL, 0x1B588D40L, 0xCCD2017FL, 0x6BB4E3BBL,
        0xDDA26A7EL, 0x3A59FF45L, 0x3E350A44L, 0xBCB4CDD5L,
        0x72EACEA8L, 0xFA6484BBL, 0x8D6612AEL, 0xBF3C6F47L,
        0xD29BE463L, 0x542F5D9EL, 0xAEC2771BL, 0xF64E6370L,
        0x740E0D8DL, 0xE75B1357L, 0xF8721671L, 0xAF537D5DL,
        0x4040CB08L, 0x4EB4E2CCL, 0x34D2466AL, 0x0115AF84L,
        0xE1B00428L, 0x95983A1DL, 0x06B89FB4L, 0xCE6EA048L,
        0x6F3F3B82L, 0x3520AB82L, 0x011A1D4BL, 0x277227F8L,
        0x611560B1L, 0xE7933FDCL, 0xBB3A792BL, 0x344525BDL,
        0xA08839E1L, 0x51CE794BL, 0x2F32C9B7L, 0xA01FBAC9L,
        0xE01CC87EL, 0xBCC7D1F6L, 0xCF0111C3L, 0xA1E8AAC7L,
        0x1A908749L, 0xD44FBD9AL, 0xD0DADECBL, 0xD50ADA38L,
        0x0339C32AL, 0xC6913667L, 0x8DF9317CL, 0xE0B12B4FL,
        0xF79E59B7L, 0x43F5BB3AL, 0xF2D519FFL, 0x27D9459CL,
        0xBF97222CL, 0x15E6FC2AL, 0x0F91FC71L, 0x9B941525L,
        0xFAE59361L, 0xCEB69CEBL, 0xC2A86459L, 0x12BAA8D1L,
        0xB6C1075EL, 0xE3056A0CL, 0x10D25065L, 0xCB03A442L,
        0xE0EC6E0EL, 0x1698DB3BL, 0x4C98A0BEL, 0x3278E964L,
        0x9F1F9532L, 0xE0D392DFL, 0xD3A0342BL, 0x8971F21EL,
        0x1B0A7441L, 0x4BA3348CL, 0xC5BE7120L, 0xC37632D8L,
        0xDF359F8DL, 0x9B992F2EL, 0xE60B6F47L, 0x0FE3F11DL,
        0xE54CDA54L, 0x1EDAD891L, 0xCE6279CFL, 0xCD3E7E6FL,
        0x1618B166L, 0xFD2C1D05L, 0x848FD2C5L, 0xF6FB2299L,
        0xF523F357L, 0xA6327623L, 0x93A83531L, 0x56CCCD02L,
        0xACF08162L, 0x5A75EBB5L, 0x6E163697L, 0x88D273CCL,
        0xDE966292L, 0x81B949D0L, 0x4C50901BL, 0x71C65614L,
        0xE6C6C7BDL, 0x327A140AL, 0x45E1D006L, 0xC3F27B9AL,
        0xC9AA53FDL, 0x62A80F00L, 0xBB25BFE2L, 0x35BDD2F6L,
        0x71126905L, 0xB2040222L, 0xB6CBCF7CL, 0xCD769C2BL,
        0x53113EC0L, 0x1640E3D3L, 0x38ABBD60L, 0x2547ADF0L,
        0xBA38209CL, 0xF746CE76L, 0x77AFA1C5L, 0x20756060L,
        0x85CBFE4EL, 0x8AE88DD8L, 0x7AAAF9B0L, 0x4CF9AA7EL,
        0x1948C25CL, 0x02FB8A8CL, 0x01C36AE4L, 0xD6EBE1F9L,
        0x90D4F869L, 0xA65CDEA0L, 0x3F09252DL, 0xC208E69FL,
        0xB74E6132L, 0xCE77E25BL, 0x578FDFE3L, 0x3AC372E6L  }
};

void CBlowFish::Test()
{
	char *cIn1 = "aAbBcCdDeE";
	char *cIn2 = "1234567890abcdefghijklmnopqrstuvwxyz";
	char cOutEncrypt[1024];
	char cOutDecrypt[1024];
	unsigned long xl, xr;
	char *buffer;

	xl = 0x01020304;
	xr = 0x10203040;

	cOutEncrypt[0] = 0;
	buffer = cOutEncrypt;
	MakeString(&buffer, &xl, &xr, 1024);
	ExtractString(&buffer, &xl, &xr);

	xl = 0x01020304;
	xr = 0x10203040;
	buffer = cIn1;
	PackFromString(&buffer, &xl, &xr);
	cOutEncrypt[0] = 0;
	buffer = cOutEncrypt;
	UnPackToString(&buffer, &xl, &xr, 1024);

	Encrypt(cIn1, cOutEncrypt,"TESTKEY", 1024);
	Decrypt(cOutEncrypt, cOutDecrypt,"TESTKEY", 1024);

	Encrypt(cIn2, cOutEncrypt,"TESTKEY",1024);
	Decrypt(cOutEncrypt, cOutDecrypt,"TESTKEY", 1024);
}

// CBC mode

void CBlowFish::Encrypt(char *pcIn, char *pcOut, char *pcKey, int iBuffer)
{
	int	iLen;
	unsigned long xl, xr, xlChain, xrChain;
	BLOWFISH_CTX ctx;

	xlChain = 0x162935;
	xrChain = 0x523709;

	iLen = (int) strlen(pcKey);

	Init (&ctx, (unsigned char*) pcKey, iLen);

	*pcOut = 0;
	while (*pcIn != 0)
	{
		PackFromString(&pcIn, &xl, &xr);

		xl = xl^xlChain;
		xr = xr^xrChain;

		Encrypt(&ctx,&xl,&xr);
		xlChain = xl;
		xrChain = xr;
		MakeString(&pcOut, &xl,&xr, iBuffer);
	}
}

// CBC mode
void CBlowFish::Decrypt(char *pcIn, char *pcOut, char *pcKey, int iBuffer)
{
	int	iLen;
	unsigned long xl, xr, xlEncrypted, xrEncrypted, xlChain, xrChain;

	BLOWFISH_CTX ctx;

	xlChain = 0x162935;
	xrChain = 0x523709;

	iLen = (int) strlen(pcKey);

	Init (&ctx, (unsigned char*) pcKey, iLen);

	*pcOut = 0;
	while (*pcIn != 0)
	{
		ExtractString(&pcIn, &xl, &xr);
		xlEncrypted = xl;
		xrEncrypted = xr;
		Decrypt(&ctx,&xl,&xr);
		xl = xl^xlChain;
		xr = xr^xrChain;
		xlChain = xlEncrypted;
		xrChain = xrEncrypted;
		UnPackToString(&pcOut, &xl, &xr, iBuffer);
	}
}

void CBlowFish::PackFromString(char **pcIn, unsigned long *xl, unsigned long *xr)
{
	unsigned long ulCh;

	*xl = 0;
	*xr = 0;

	// xl
	for (int iPos = 0; iPos < 4; iPos++)
	{
		if (**pcIn == 0) return;
		ulCh = **pcIn;
		(*xl) += ulCh << 8*iPos;
		(*pcIn)++;
	}
	// xr
	for (int iPos = 0; iPos < 4; iPos++)
	{
		if (**pcIn == 0) return;
		ulCh = **pcIn;
		(*xr) += ulCh << 8*iPos;
		(*pcIn)++;
	}
}

void CBlowFish::UnPackToString(char **pcOut, unsigned long *xl, unsigned long *xr, int iBuffer)
{
	char	cString[11];
	int		iPos;

	// xl
	for (iPos = 0; iPos < 4; iPos++)
	{
		cString[iPos] = (char) ((*xl) & 0xff);
		*xl = (*xl) >> 8;
	}
	// xr
	for (; iPos < 8; iPos++)
	{
		cString[iPos] = (char) ((*xr) & 0xff);
		*xr = (*xr) >> 8;
	}
	cString[iPos] = 0;
	strcat_s(*pcOut, iBuffer, cString);
}

void CBlowFish::MakeString(char **pcOut, unsigned long *xl, unsigned long *xr, int iBuffer)
{
	int		iPos;
	char	cString[11];
	__int64	i64;

	i64 = *xl;
	i64  = i64 << 32;
	i64+= *xr;

	for (iPos = 0; iPos < 10; iPos++)
	{
		cString[iPos] = (i64 & 0x7f) + 1;
		i64 = i64 >> 7;
	}
	cString[iPos] = 0;

	strcat_s(*pcOut, iBuffer, cString);
}

void CBlowFish::ExtractString(char **pcIn, unsigned long *xl, unsigned long *xr)
{
	__int64	i64, i64Temp;
	int		iPos, iShift;

	i64 = 0;
	iShift = 0;

	for (iPos = 0; iPos < 10; iPos++)
	{
		if (**pcIn == 0) return;
		i64Temp = (**pcIn)-1;
		i64Temp&= 0x7f;
		i64Temp = i64Temp << iShift;
		iShift += 7;
		i64 += i64Temp;
		(*pcIn)++;	
	}

	*xl = (i64>>32) & 0xffffffff;
	*xr = i64 & 0xffffffff;
}

void CBlowFish::MakeHex(unsigned char *pcEncrypted, char *pcHex, int iLen)
{
	char cBuffer[16];
//char *pcEncryptedBegin;
	unsigned int	ui;

//	char cEncryptedBegin[256];

//	strcpy(cEncryptedBegin, (char *) pcEncrypted);

	while (*pcEncrypted)
	{
		ui = *pcEncrypted;
		_snprintf_s(cBuffer,16,_TRUNCATE,"%02X", ui);
		strcat_s(pcHex,iLen,cBuffer);
		pcEncrypted++;
	}
}

void CBlowFish::ExtractHex(char *pcEncrypted, char *pcHex, int iLen)
{
	int iCount = 0;
	char cBuffer[16];
	unsigned int	ui;
//	ui = 0;
	*pcEncrypted = 0;
	while (iCount < iLen)
	{
		if (*pcHex == 0) return;
		cBuffer[0] = *pcHex;
		pcHex++;
		if (*pcHex == 0) return;
		cBuffer[1] = *pcHex;
		pcHex++;
		cBuffer[2] = 0;
		iCount+=2;
		sscanf_s(cBuffer, "%X", &ui, 16);
		*pcEncrypted = (char) ui;
		pcEncrypted++;
		*pcEncrypted = 0;
	}

}




unsigned long CBlowFish::F(BLOWFISH_CTX *ctx, unsigned long x)
{
   unsigned short a, b, c, d;
   unsigned long  y;

   d = (unsigned short)(x & 0xFF);
   x >>= 8;
   c = (unsigned short)(x & 0xFF);
   x >>= 8;
   b = (unsigned short)(x & 0xFF);
   x >>= 8;
   a = (unsigned short)(x & 0xFF);
   y = ctx->S[0][a] + ctx->S[1][b];
   y = y ^ ctx->S[2][c];
   y = y + ctx->S[3][d];

   return y;
}


void CBlowFish::Encrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr)
{
  unsigned long  Xl;
  unsigned long  Xr;
  unsigned long  temp;
  short       i;

  Xl = *xl;
  Xr = *xr;

  for (i = 0; i < BLOWFISH_NUMBER; ++i) {
    Xl = Xl ^ ctx->P[i];
    Xr = F(ctx, Xl) ^ Xr;

    temp = Xl;
    Xl = Xr;
    Xr = temp;
  }

  temp = Xl;
  Xl = Xr;
  Xr = temp;

  Xr = Xr ^ ctx->P[BLOWFISH_NUMBER];
  Xl = Xl ^ ctx->P[BLOWFISH_NUMBER + 1];

  *xl = Xl;
  *xr = Xr;
}


void CBlowFish::Decrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr)
{
  unsigned long  Xl;
  unsigned long  Xr;
  unsigned long  temp;
  short       i;

  Xl = *xl;
  Xr = *xr;

  for (i = BLOWFISH_NUMBER + 1; i > 1; --i) {
    Xl = Xl ^ ctx->P[i];
    Xr = F(ctx, Xl) ^ Xr;

    /* Exchange Xl and Xr */
    temp = Xl;
    Xl = Xr;
    Xr = temp;
  }

  /* Exchange Xl and Xr */
  temp = Xl;
  Xl = Xr;
  Xr = temp;

  Xr = Xr ^ ctx->P[1];
  Xl = Xl ^ ctx->P[0];

  *xl = Xl;
  *xr = Xr;
}


void CBlowFish::Init(BLOWFISH_CTX *ctx, unsigned char *key, int keyLen)
{
  int i, j, k;
  unsigned long data, datal, datar;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 256; j++)
      ctx->S[i][j] = BLOWFISH_ORIG_S[i][j];
  }

  j = 0;
  for (i = 0; i < BLOWFISH_NUMBER + 2; ++i) {
    data = 0x00000000;
    for (k = 0; k < 4; ++k) {
      data = (data << 8) | key[j];
      j = j + 1;
      if (j >= keyLen)
        j = 0;
    }
    ctx->P[i] = BLOWFISH_ORIG_P[i] ^ data;
  }

  datal = 0x00000000;
  datar = 0x00000000;

  for (i = 0; i < BLOWFISH_NUMBER + 2; i += 2) {
    Encrypt(ctx, &datal, &datar);
    ctx->P[i] = datal;
    ctx->P[i + 1] = datar;
  }

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 256; j += 2) {
      Encrypt(ctx, &datal, &datar);
      ctx->S[i][j] = datal;
      ctx->S[i][j + 1] = datar;
    }
  }
}


// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


/*

//#using <mscorlib.dll>
#include "StdAfx.h"
#include "blowfish.h"
#include <cstring>
#include <exception>



const unsigned int CBlowFish::scm_auiInitP[18] = {
	0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344,
		0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89,
		0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c,
		0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
		0x9216d5d9, 0x8979fb1b
	};

const unsigned int CBlowFish::scm_auiInitS[4][256] = {
	//0
	{0xd1310ba6, 0x98dfb5ac, 0x2ffd72db, 0xd01adfb7,
		0xb8e1afed, 0x6a267e96, 0xba7c9045, 0xf12c7f99,
		0x24a19947, 0xb3916cf7, 0x0801f2e2, 0x858efc16,
		0x636920d8, 0x71574e69, 0xa458fea3, 0xf4933d7e,
		0x0d95748f, 0x728eb658, 0x718bcd58, 0x82154aee,
		0x7b54a41d, 0xc25a59b5, 0x9c30d539, 0x2af26013,
		0xc5d1b023, 0x286085f0, 0xca417918, 0xb8db38ef,
		0x8e79dcb0, 0x603a180e, 0x6c9e0e8b, 0xb01e8a3e,
		0xd71577c1, 0xbd314b27, 0x78af2fda, 0x55605c60,
		0xe65525f3, 0xaa55ab94, 0x57489862, 0x63e81440,
		0x55ca396a, 0x2aab10b6, 0xb4cc5c34, 0x1141e8ce,
		0xa15486af, 0x7c72e993, 0xb3ee1411, 0x636fbc2a,
		0x2ba9c55d, 0x741831f6, 0xce5c3e16, 0x9b87931e,
		0xafd6ba33, 0x6c24cf5c, 0x7a325381, 0x28958677,
		0x3b8f4898, 0x6b4bb9af, 0xc4bfe81b, 0x66282193,
		0x61d809cc, 0xfb21a991, 0x487cac60, 0x5dec8032,
		0xef845d5d, 0xe98575b1, 0xdc262302, 0xeb651b88,
		0x23893e81, 0xd396acc5, 0x0f6d6ff3, 0x83f44239,
		0x2e0b4482, 0xa4842004, 0x69c8f04a, 0x9e1f9b5e,
		0x21c66842, 0xf6e96c9a, 0x670c9c61, 0xabd388f0,
		0x6a51a0d2, 0xd8542f68, 0x960fa728, 0xab5133a3,
		0x6eef0b6c, 0x137a3be4, 0xba3bf050, 0x7efb2a98,
		0xa1f1651d, 0x39af0176, 0x66ca593e, 0x82430e88,
		0x8cee8619, 0x456f9fb4, 0x7d84a5c3, 0x3b8b5ebe,
		0xe06f75d8, 0x85c12073, 0x401a449f, 0x56c16aa6,
		0x4ed3aa62, 0x363f7706, 0x1bfedf72, 0x429b023d,
		0x37d0d724, 0xd00a1248, 0xdb0fead3, 0x49f1c09b,
		0x075372c9, 0x80991b7b, 0x25d479d8, 0xf6e8def7,
		0xe3fe501a, 0xb6794c3b, 0x976ce0bd, 0x04c006ba,
		0xc1a94fb6, 0x409f60c4, 0x5e5c9ec2, 0x196a2463,
		0x68fb6faf, 0x3e6c53b5, 0x1339b2eb, 0x3b52ec6f,
		0x6dfc511f, 0x9b30952c, 0xcc814544, 0xaf5ebd09,
		0xbee3d004, 0xde334afd, 0x660f2807, 0x192e4bb3,
		0xc0cba857, 0x45c8740f, 0xd20b5f39, 0xb9d3fbdb,
		0x5579c0bd, 0x1a60320a, 0xd6a100c6, 0x402c7279,
		0x679f25fe, 0xfb1fa3cc, 0x8ea5e9f8, 0xdb3222f8,
		0x3c7516df, 0xfd616b15, 0x2f501ec8, 0xad0552ab,
		0x323db5fa, 0xfd238760, 0x53317b48, 0x3e00df82,
		0x9e5c57bb, 0xca6f8ca0, 0x1a87562e, 0xdf1769db,
		0xd542a8f6, 0x287effc3, 0xac6732c6, 0x8c4f5573,
		0x695b27b0, 0xbbca58c8, 0xe1ffa35d, 0xb8f011a0,
		0x10fa3d98, 0xfd2183b8, 0x4afcb56c, 0x2dd1d35b,
		0x9a53e479, 0xb6f84565, 0xd28e49bc, 0x4bfb9790,
		0xe1ddf2da, 0xa4cb7e33, 0x62fb1341, 0xcee4c6e8,
		0xef20cada, 0x36774c01, 0xd07e9efe, 0x2bf11fb4,
		0x95dbda4d, 0xae909198, 0xeaad8e71, 0x6b93d5a0,
		0xd08ed1d0, 0xafc725e0, 0x8e3c5b2f, 0x8e7594b7,
		0x8ff6e2fb, 0xf2122b64, 0x8888b812, 0x900df01c,
		0x4fad5ea0, 0x688fc31c, 0xd1cff191, 0xb3a8c1ad,
		0x2f2f2218, 0xbe0e1777, 0xea752dfe, 0x8b021fa1,
		0xe5a0cc0f, 0xb56f74e8, 0x18acf3d6, 0xce89e299,
		0xb4a84fe0, 0xfd13e0b7, 0x7cc43b81, 0xd2ada8d9,
		0x165fa266, 0x80957705, 0x93cc7314, 0x211a1477,
		0xe6ad2065, 0x77b5fa86, 0xc75442f5, 0xfb9d35cf,
		0xebcdaf0c, 0x7b3e89a0, 0xd6411bd3, 0xae1e7e49,
		0x00250e2d, 0x2071b35e, 0x226800bb, 0x57b8e0af,
		0x2464369b, 0xf009b91e, 0x5563911d, 0x59dfa6aa,
		0x78c14389, 0xd95a537f, 0x207d5ba2, 0x02e5b9c5,
		0x83260376, 0x6295cfa9, 0x11c81968, 0x4e734a41,
		0xb3472dca, 0x7b14a94a, 0x1b510052, 0x9a532915,
		0xd60f573f, 0xbc9bc6e4, 0x2b60a476, 0x81e67400,
		0x08ba6fb5, 0x571be91f, 0xf296ec6b, 0x2a0dd915,
		0xb6636521, 0xe7b9f9b6, 0xff34052e, 0xc5855664,
		0x53b02d5d, 0xa99f8fa1, 0x08ba4799, 0x6e85076a},

		//1
	{0x4b7a70e9, 0xb5b32944, 0xdb75092e, 0xc4192623,
	0xad6ea6b0, 0x49a7df7d, 0x9cee60b8, 0x8fedb266,
	0xecaa8c71, 0x699a17ff, 0x5664526c, 0xc2b19ee1,
	0x193602a5, 0x75094c29, 0xa0591340, 0xe4183a3e,
	0x3f54989a, 0x5b429d65, 0x6b8fe4d6, 0x99f73fd6,
	0xa1d29c07, 0xefe830f5, 0x4d2d38e6, 0xf0255dc1,
	0x4cdd2086, 0x8470eb26, 0x6382e9c6, 0x021ecc5e,
	0x09686b3f, 0x3ebaefc9, 0x3c971814, 0x6b6a70a1,
	0x687f3584, 0x52a0e286, 0xb79c5305, 0xaa500737,
	0x3e07841c, 0x7fdeae5c, 0x8e7d44ec, 0x5716f2b8,
	0xb03ada37, 0xf0500c0d, 0xf01c1f04, 0x0200b3ff,
	0xae0cf51a, 0x3cb574b2, 0x25837a58, 0xdc0921bd,
	0xd19113f9, 0x7ca92ff6, 0x94324773, 0x22f54701,
	0x3ae5e581, 0x37c2dadc, 0xc8b57634, 0x9af3dda7,
	0xa9446146, 0x0fd0030e, 0xecc8c73e, 0xa4751e41,
	0xe238cd99, 0x3bea0e2f, 0x3280bba1, 0x183eb331,
	0x4e548b38, 0x4f6db908, 0x6f420d03, 0xf60a04bf,
	0x2cb81290, 0x24977c79, 0x5679b072, 0xbcaf89af,
	0xde9a771f, 0xd9930810, 0xb38bae12, 0xdccf3f2e,
	0x5512721f, 0x2e6b7124, 0x501adde6, 0x9f84cd87,
	0x7a584718, 0x7408da17, 0xbc9f9abc, 0xe94b7d8c,
	0xec7aec3a, 0xdb851dfa, 0x63094366, 0xc464c3d2,
	0xef1c1847, 0x3215d908, 0xdd433b37, 0x24c2ba16,
	0x12a14d43, 0x2a65c451, 0x50940002, 0x133ae4dd,
	0x71dff89e, 0x10314e55, 0x81ac77d6, 0x5f11199b,
	0x043556f1, 0xd7a3c76b, 0x3c11183b, 0x5924a509,
	0xf28fe6ed, 0x97f1fbfa, 0x9ebabf2c, 0x1e153c6e,
	0x86e34570, 0xeae96fb1, 0x860e5e0a, 0x5a3e2ab3,
	0x771fe71c, 0x4e3d06fa, 0x2965dcb9, 0x99e71d0f,
	0x803e89d6, 0x5266c825, 0x2e4cc978, 0x9c10b36a,
	0xc6150eba, 0x94e2ea78, 0xa5fc3c53, 0x1e0a2df4,
	0xf2f74ea7, 0x361d2b3d, 0x1939260f, 0x19c27960,
	0x5223a708, 0xf71312b6, 0xebadfe6e, 0xeac31f66,
	0xe3bc4595, 0xa67bc883, 0xb17f37d1, 0x018cff28,
	0xc332ddef, 0xbe6c5aa5, 0x65582185, 0x68ab9802,
	0xeecea50f, 0xdb2f953b, 0x2aef7dad, 0x5b6e2f84,
	0x1521b628, 0x29076170, 0xecdd4775, 0x619f1510,
	0x13cca830, 0xeb61bd96, 0x0334fe1e, 0xaa0363cf,
	0xb5735c90, 0x4c70a239, 0xd59e9e0b, 0xcbaade14,
	0xeecc86bc, 0x60622ca7, 0x9cab5cab, 0xb2f3846e,
	0x648b1eaf, 0x19bdf0ca, 0xa02369b9, 0x655abb50,
	0x40685a32, 0x3c2ab4b3, 0x319ee9d5, 0xc021b8f7,
	0x9b540b19, 0x875fa099, 0x95f7997e, 0x623d7da8,
	0xf837889a, 0x97e32d77, 0x11ed935f, 0x16681281,
	0x0e358829, 0xc7e61fd6, 0x96dedfa1, 0x7858ba99,
	0x57f584a5, 0x1b227263, 0x9b83c3ff, 0x1ac24696,
	0xcdb30aeb, 0x532e3054, 0x8fd948e4, 0x6dbc3128,
	0x58ebf2ef, 0x34c6ffea, 0xfe28ed61, 0xee7c3c73,
	0x5d4a14d9, 0xe864b7e3, 0x42105d14, 0x203e13e0,
	0x45eee2b6, 0xa3aaabea, 0xdb6c4f15, 0xfacb4fd0,
	0xc742f442, 0xef6abbb5, 0x654f3b1d, 0x41cd2105,
	0xd81e799e, 0x86854dc7, 0xe44b476a, 0x3d816250,
	0xcf62a1f2, 0x5b8d2646, 0xfc8883a0, 0xc1c7b6a3,
	0x7f1524c3, 0x69cb7492, 0x47848a0b, 0x5692b285,
	0x095bbf00, 0xad19489d, 0x1462b174, 0x23820e00,
	0x58428d2a, 0x0c55f5ea, 0x1dadf43e, 0x233f7061,
	0x3372f092, 0x8d937e41, 0xd65fecf1, 0x6c223bdb,
	0x7cde3759, 0xcbee7460, 0x4085f2a7, 0xce77326e,
	0xa6078084, 0x19f8509e, 0xe8efd855, 0x61d99735,
	0xa969a7aa, 0xc50c06c2, 0x5a04abfc, 0x800bcadc,
	0x9e447a2e, 0xc3453484, 0xfdd56705, 0x0e1e9ec9,
	0xdb73dbd3, 0x105588cd, 0x675fda79, 0xe3674340,
	0xc5c43465, 0x713e38d8, 0x3d28f89e, 0xf16dff20,
	0x153e21e7, 0x8fb03d4a, 0xe6e39f2b, 0xdb83adf7},

	//2
	{0xe93d5a68, 0x948140f7, 0xf64c261c, 0x94692934,
	0x411520f7, 0x7602d4f7, 0xbcf46b2e, 0xd4a20068,
	0xd4082471, 0x3320f46a, 0x43b7d4b7, 0x500061af,
	0x1e39f62e, 0x97244546, 0x14214f74, 0xbf8b8840,
	0x4d95fc1d, 0x96b591af, 0x70f4ddd3, 0x66a02f45,
	0xbfbc09ec, 0x03bd9785, 0x7fac6dd0, 0x31cb8504,
	0x96eb27b3, 0x55fd3941, 0xda2547e6, 0xabca0a9a,
	0x28507825, 0x530429f4, 0x0a2c86da, 0xe9b66dfb,
	0x68dc1462, 0xd7486900, 0x680ec0a4, 0x27a18dee,
	0x4f3ffea2, 0xe887ad8c, 0xb58ce006, 0x7af4d6b6,
	0xaace1e7c, 0xd3375fec, 0xce78a399, 0x406b2a42,
	0x20fe9e35, 0xd9f385b9, 0xee39d7ab, 0x3b124e8b,
	0x1dc9faf7, 0x4b6d1856, 0x26a36631, 0xeae397b2,
	0x3a6efa74, 0xdd5b4332, 0x6841e7f7, 0xca7820fb,
	0xfb0af54e, 0xd8feb397, 0x454056ac, 0xba489527,
	0x55533a3a, 0x20838d87, 0xfe6ba9b7, 0xd096954b,
	0x55a867bc, 0xa1159a58, 0xcca92963, 0x99e1db33,
	0xa62a4a56, 0x3f3125f9, 0x5ef47e1c, 0x9029317c,
	0xfdf8e802, 0x04272f70, 0x80bb155c, 0x05282ce3,
	0x95c11548, 0xe4c66d22, 0x48c1133f, 0xc70f86dc,
	0x07f9c9ee, 0x41041f0f, 0x404779a4, 0x5d886e17,
	0x325f51eb, 0xd59bc0d1, 0xf2bcc18f, 0x41113564,
	0x257b7834, 0x602a9c60, 0xdff8e8a3, 0x1f636c1b,
	0x0e12b4c2, 0x02e1329e, 0xaf664fd1, 0xcad18115,
	0x6b2395e0, 0x333e92e1, 0x3b240b62, 0xeebeb922,
	0x85b2a20e, 0xe6ba0d99, 0xde720c8c, 0x2da2f728,
	0xd0127845, 0x95b794fd, 0x647d0862, 0xe7ccf5f0,
	0x5449a36f, 0x877d48fa, 0xc39dfd27, 0xf33e8d1e,
	0x0a476341, 0x992eff74, 0x3a6f6eab, 0xf4f8fd37,
	0xa812dc60, 0xa1ebddf8, 0x991be14c, 0xdb6e6b0d,
	0xc67b5510, 0x6d672c37, 0x2765d43b, 0xdcd0e804,
	0xf1290dc7, 0xcc00ffa3, 0xb5390f92, 0x690fed0b,
	0x667b9ffb, 0xcedb7d9c, 0xa091cf0b, 0xd9155ea3,
	0xbb132f88, 0x515bad24, 0x7b9479bf, 0x763bd6eb,
	0x37392eb3, 0xcc115979, 0x8026e297, 0xf42e312d,
	0x6842ada7, 0xc66a2b3b, 0x12754ccc, 0x782ef11c,
	0x6a124237, 0xb79251e7, 0x06a1bbe6, 0x4bfb6350,
	0x1a6b1018, 0x11caedfa, 0x3d25bdd8, 0xe2e1c3c9,
	0x44421659, 0x0a121386, 0xd90cec6e, 0xd5abea2a,
	0x64af674e, 0xda86a85f, 0xbebfe988, 0x64e4c3fe,
	0x9dbc8057, 0xf0f7c086, 0x60787bf8, 0x6003604d,
	0xd1fd8346, 0xf6381fb0, 0x7745ae04, 0xd736fccc,
	0x83426b33, 0xf01eab71, 0xb0804187, 0x3c005e5f,
	0x77a057be, 0xbde8ae24, 0x55464299, 0xbf582e61,
	0x4e58f48f, 0xf2ddfda2, 0xf474ef38, 0x8789bdc2,
	0x5366f9c3, 0xc8b38e74, 0xb475f255, 0x46fcd9b9,
	0x7aeb2661, 0x8b1ddf84, 0x846a0e79, 0x915f95e2,
	0x466e598e, 0x20b45770, 0x8cd55591, 0xc902de4c,
	0xb90bace1, 0xbb8205d0, 0x11a86248, 0x7574a99e,
	0xb77f19b6, 0xe0a9dc09, 0x662d09a1, 0xc4324633,
	0xe85a1f02, 0x09f0be8c, 0x4a99a025, 0x1d6efe10,
	0x1ab93d1d, 0x0ba5a4df, 0xa186f20f, 0x2868f169,
	0xdcb7da83, 0x573906fe, 0xa1e2ce9b, 0x4fcd7f52,
	0x50115e01, 0xa70683fa, 0xa002b5c4, 0x0de6d027,
	0x9af88c27, 0x773f8641, 0xc3604c06, 0x61a806b5,
	0xf0177a28, 0xc0f586e0, 0x006058aa, 0x30dc7d62,
	0x11e69ed7, 0x2338ea63, 0x53c2dd94, 0xc2c21634,
	0xbbcbee56, 0x90bcb6de, 0xebfc7da1, 0xce591d76,
	0x6f05e409, 0x4b7c0188, 0x39720a3d, 0x7c927c24,
	0x86e3725f, 0x724d9db9, 0x1ac15bb4, 0xd39eb8fc,
	0xed545578, 0x08fca5b5, 0xd83d7cd3, 0x4dad0fc4,
	0x1e50ef5e, 0xb161e6f8, 0xa28514d9, 0x6c51133c,
	0x6fd5c7e7, 0x56e14ec4, 0x362abfce, 0xddc6c837,
	0xd79a3234, 0x92638212, 0x670efa8e, 0x406000e0},

	//3
	{0x3a39ce37, 0xd3faf5cf, 0xabc27737, 0x5ac52d1b,
	0x5cb0679e, 0x4fa33742, 0xd3822740, 0x99bc9bbe,
	0xd5118e9d, 0xbf0f7315, 0xd62d1c7e, 0xc700c47b,
	0xb78c1b6b, 0x21a19045, 0xb26eb1be, 0x6a366eb4,
	0x5748ab2f, 0xbc946e79, 0xc6a376d2, 0x6549c2c8,
	0x530ff8ee, 0x468dde7d, 0xd5730a1d, 0x4cd04dc6,
	0x2939bbdb, 0xa9ba4650, 0xac9526e8, 0xbe5ee304,
	0xa1fad5f0, 0x6a2d519a, 0x63ef8ce2, 0x9a86ee22,
	0xc089c2b8, 0x43242ef6, 0xa51e03aa, 0x9cf2d0a4,
	0x83c061ba, 0x9be96a4d, 0x8fe51550, 0xba645bd6,
	0x2826a2f9, 0xa73a3ae1, 0x4ba99586, 0xef5562e9,
	0xc72fefd3, 0xf752f7da, 0x3f046f69, 0x77fa0a59,
	0x80e4a915, 0x87b08601, 0x9b09e6ad, 0x3b3ee593,
	0xe990fd5a, 0x9e34d797, 0x2cf0b7d9, 0x022b8b51,
	0x96d5ac3a, 0x017da67d, 0xd1cf3ed6, 0x7c7d2d28,
	0x1f9f25cf, 0xadf2b89b, 0x5ad6b472, 0x5a88f54c,
	0xe029ac71, 0xe019a5e6, 0x47b0acfd, 0xed93fa9b,
	0xe8d3c48d, 0x283b57cc, 0xf8d56629, 0x79132e28,
	0x785f0191, 0xed756055, 0xf7960e44, 0xe3d35e8c,
	0x15056dd4, 0x88f46dba, 0x03a16125, 0x0564f0bd,
	0xc3eb9e15, 0x3c9057a2, 0x97271aec, 0xa93a072a,
	0x1b3f6d9b, 0x1e6321f5, 0xf59c66fb, 0x26dcf319,
	0x7533d928, 0xb155fdf5, 0x03563482, 0x8aba3cbb,
	0x28517711, 0xc20ad9f8, 0xabcc5167, 0xccad925f,
	0x4de81751, 0x3830dc8e, 0x379d5862, 0x9320f991,
	0xea7a90c2, 0xfb3e7bce, 0x5121ce64, 0x774fbe32,
	0xa8b6e37e, 0xc3293d46, 0x48de5369, 0x6413e680,
	0xa2ae0810, 0xdd6db224, 0x69852dfd, 0x09072166,
	0xb39a460a, 0x6445c0dd, 0x586cdecf, 0x1c20c8ae,
	0x5bbef7dd, 0x1b588d40, 0xccd2017f, 0x6bb4e3bb,
	0xdda26a7e, 0x3a59ff45, 0x3e350a44, 0xbcb4cdd5,
	0x72eacea8, 0xfa6484bb, 0x8d6612ae, 0xbf3c6f47,
	0xd29be463, 0x542f5d9e, 0xaec2771b, 0xf64e6370,
	0x740e0d8d, 0xe75b1357, 0xf8721671, 0xaf537d5d,
	0x4040cb08, 0x4eb4e2cc, 0x34d2466a, 0x0115af84,
	0xe1b00428, 0x95983a1d, 0x06b89fb4, 0xce6ea048,
	0x6f3f3b82, 0x3520ab82, 0x011a1d4b, 0x277227f8,
	0x611560b1, 0xe7933fdc, 0xbb3a792b, 0x344525bd,
	0xa08839e1, 0x51ce794b, 0x2f32c9b7, 0xa01fbac9,
	0xe01cc87e, 0xbcc7d1f6, 0xcf0111c3, 0xa1e8aac7,
	0x1a908749, 0xd44fbd9a, 0xd0dadecb, 0xd50ada38,
	0x0339c32a, 0xc6913667, 0x8df9317c, 0xe0b12b4f,
	0xf79e59b7, 0x43f5bb3a, 0xf2d519ff, 0x27d9459c,
	0xbf97222c, 0x15e6fc2a, 0x0f91fc71, 0x9b941525,
	0xfae59361, 0xceb69ceb, 0xc2a86459, 0x12baa8d1,
	0xb6c1075e, 0xe3056a0c, 0x10d25065, 0xcb03a442,
	0xe0ec6e0e, 0x1698db3b, 0x4c98a0be, 0x3278e964,
	0x9f1f9532, 0xe0d392df, 0xd3a0342b, 0x8971f21e,
	0x1b0a7441, 0x4ba3348c, 0xc5be7120, 0xc37632d8,
	0xdf359f8d, 0x9b992f2e, 0xe60b6f47, 0x0fe3f11d,
	0xe54cda54, 0x1edad891, 0xce6279cf, 0xcd3e7e6f,
	0x1618b166, 0xfd2c1d05, 0x848fd2c5, 0xf6fb2299,
	0xf523f357, 0xa6327623, 0x93a83531, 0x56cccd02,
	0xacf08162, 0x5a75ebb5, 0x6e163697, 0x88d273cc,
	0xde966292, 0x81b949d0, 0x4c50901b, 0x71c65614,
	0xe6c6c7bd, 0x327a140a, 0x45e1d006, 0xc3f27b9a,
	0xc9aa53fd, 0x62a80f00, 0xbb25bfe2, 0x35bdd2f6,
	0x71126905, 0xb2040222, 0xb6cbcf7c, 0xcd769c2b,
	0x53113ec0, 0x1640e3d3, 0x38abbd60, 0x2547adf0,
	0xba38209c, 0xf746ce76, 0x77afa1c5, 0x20756060,
	0x85cbfe4e, 0x8ae88dd8, 0x7aaaf9b0, 0x4cf9aa7e,
	0x1948c25c, 0x02fb8a8c, 0x01c36ae4, 0xd6ebe1f9,
	0x90d4f869, 0xa65cdea0, 0x3f09252d, 0xc208e69f,
	0xb74e6132, 0xce77e25b, 0x578fdfe3, 0x3ac372e6}
	};

//Constructor - Initialize the P and S boxes for a given Key
CBlowFish::CBlowFish(unsigned char* ucKey, size_t keysize, const SBlock& roChain) : m_oChain0(roChain), m_oChain(roChain)
	{
	if(keysize<1) return;
//		throw exception("Incorrect buffer length");
	//Check the Key - the key length should be between 1 and 56 bytes
	if(keysize>56)
		keysize = 56;
	unsigned char aucLocalKey[56];
	unsigned int i, j;
	memcpy(aucLocalKey, ucKey, keysize);
	//Reflexive Initialization of the Blowfish.
	//Generating the Subkeys from the Key flood P and S boxes with PI
	memcpy(m_auiP, scm_auiInitP, sizeof m_auiP);
	memcpy(m_auiS, scm_auiInitS, sizeof m_auiS);
	//Load P boxes with key bytes
	const unsigned char* p = aucLocalKey;
	unsigned int x=0;
	//Repeatedly cycle through the key bits until the entire P array has been XORed with key bits
	int iCount = 0;
	for(i=0; i<18; i++)
		{
		x=0;
		for(int n=4; n--; )
			{
			int iVal = (int)(*p);
			x <<= 8;
			x |= *(p++);
			iCount++;
			if(iCount == keysize)
				{
				//All bytes used, so recycle bytes 
				iCount = 0;
				p = aucLocalKey;
				}
			}
		m_auiP[i] ^= x;
		}
	//Reflect P and S boxes through the evolving Blowfish
	SBlock block(0UL,0UL); //all-zero block
	for(i=0; i<18; )
		Encrypt(block), m_auiP[i++] = block.m_uil, m_auiP[i++] = block.m_uir;
	for(j=0; j<4; j++)
		for(int k=0; k<256; )
			Encrypt(block), m_auiS[j][k++] = block.m_uil, m_auiS[j][k++] = block.m_uir;
	}

//Sixteen Round Encipher of Block
void CBlowFish::Encrypt(SBlock& block)
	{
	unsigned int uiLeft = block.m_uil;
	unsigned int uiRight = block.m_uir;
	uiLeft ^= m_auiP[0];
	uiRight ^= F(uiLeft)^m_auiP[1]; uiLeft ^= F(uiRight)^m_auiP[2];
	uiRight ^= F(uiLeft)^m_auiP[3]; uiLeft ^= F(uiRight)^m_auiP[4];
	uiRight ^= F(uiLeft)^m_auiP[5]; uiLeft ^= F(uiRight)^m_auiP[6];
	uiRight ^= F(uiLeft)^m_auiP[7]; uiLeft ^= F(uiRight)^m_auiP[8];
	uiRight ^= F(uiLeft)^m_auiP[9]; uiLeft ^= F(uiRight)^m_auiP[10];
	uiRight ^= F(uiLeft)^m_auiP[11]; uiLeft ^= F(uiRight)^m_auiP[12];
	uiRight ^= F(uiLeft)^m_auiP[13]; uiLeft ^= F(uiRight)^m_auiP[14];
	uiRight ^= F(uiLeft)^m_auiP[15]; uiLeft ^= F(uiRight)^m_auiP[16];
	uiRight ^= m_auiP[17];
	block.m_uil = uiRight;
	block.m_uir = uiLeft;
	}

//Sixteen Round Decipher of SBlock
void CBlowFish::Decrypt(SBlock& block)
	{
	unsigned int uiLeft = block.m_uil;
	unsigned int uiRight = block.m_uir;
	uiLeft ^= m_auiP[17];
	uiRight ^= F(uiLeft)^m_auiP[16]; uiLeft ^= F(uiRight)^m_auiP[15];
	uiRight ^= F(uiLeft)^m_auiP[14]; uiLeft ^= F(uiRight)^m_auiP[13];
	uiRight ^= F(uiLeft)^m_auiP[12]; uiLeft ^= F(uiRight)^m_auiP[11];
	uiRight ^= F(uiLeft)^m_auiP[10]; uiLeft ^= F(uiRight)^m_auiP[9];
	uiRight ^= F(uiLeft)^m_auiP[8]; uiLeft ^= F(uiRight)^m_auiP[7];
	uiRight ^= F(uiLeft)^m_auiP[6]; uiLeft ^= F(uiRight)^m_auiP[5];
	uiRight ^= F(uiLeft)^m_auiP[4]; uiLeft ^= F(uiRight)^m_auiP[3];
	uiRight ^= F(uiLeft)^m_auiP[2]; uiLeft ^= F(uiRight)^m_auiP[1];
	uiRight ^= m_auiP[0];
	block.m_uil = uiRight;
	block.m_uir = uiLeft;
	}

//Semi-Portable Byte Shuffling
inline void BytesToBlock(unsigned char const* p, SBlock& b)
	{
	unsigned int y;
	//Left
	b.m_uil = 0;
	y = *p++;
	y <<= 24;
	b.m_uil |= y;
	y = *p++;
	y <<= 16;
	b.m_uil |= y;
	y = *p++;
	y <<= 8;
	b.m_uil |= y;
	y = *p++;
	b.m_uil |= y;
	//Right
	b.m_uir = 0;
	y = *p++;
	y <<= 24;
	b.m_uir |= y;
	y = *p++;
	y <<= 16;
	b.m_uir |= y;
	y = *p++;
	y <<= 8;
	b.m_uir |= y;
	y = *p++;
	b.m_uir |= y;
	}

inline void BlockToBytes(SBlock const& b, unsigned char* p)
	{
	unsigned int y;
	//Right
	y = b.m_uir;
	*--p = Byte(y);
	y = b.m_uir >> 8;
	*--p = Byte(y);
	y = b.m_uir >> 16;
	*--p = Byte(y);
	y = b.m_uir >> 24;
	*--p = Byte(y);
	//Left
	y = b.m_uil;
	*--p = Byte(y);
	y = b.m_uil >> 8;
	*--p = Byte(y);
	y = b.m_uil >> 16;
	*--p = Byte(y);
	y = b.m_uil >> 24;
	*--p = Byte(y);
	}

//Encrypt Buffer in Place
//Returns false if n is multiple of 8
void CBlowFish::Encrypt(unsigned char* buf, size_t n, int iMode)
	{
	//Check the buffer's length - should be > 0 and multiple of 8
	if((n==0)||(n%8!=0)) return;
//		throw exception("Incorrect buffer length");
	SBlock work;
	if(iMode == CBC) //CBC mode, using the Chain
		{
		SBlock chain(m_oChain);
		for(; n >= 8; n -= 8)
			{
			BytesToBlock(buf, work);
			work ^= chain;
			Encrypt(work);
			chain = work;
			BlockToBytes(work, buf+=8);
			}
		}
	else if(iMode == CFB) //CFB mode, using the Chain
		{
		SBlock chain(m_oChain);
		for(; n >= 8; n -= 8)
			{
			Encrypt(chain);
			BytesToBlock(buf, work);
			work ^= chain;
			chain = work;
			BlockToBytes(work, buf+=8);
			}
		}
	else //ECB mode, not using the Chain
		{
		for(; n >= 8; n -= 8)
			{
			BytesToBlock(buf, work);
			Encrypt(work);
			BlockToBytes(work, buf+=8);
			}
		}
	}

//Decrypt Buffer in Place
//Returns false if n is multiple of 8
void CBlowFish::Decrypt(unsigned char* buf, size_t n, int iMode)
	{
	//Check the buffer's length - should be > 0 and multiple of 8
	if((n==0)||(n%8!=0)) return;
//		throw exception("Incorrect buffer length");
	SBlock work;
	if(iMode == CBC) //CBC mode, using the Chain
		{
		SBlock crypt, chain(m_oChain);
		for(; n >= 8; n -= 8)
			{
			BytesToBlock(buf, work);
			crypt = work;
			Decrypt(work);
			work ^= chain;
			chain = crypt;
			BlockToBytes(work, buf+=8);
			}
		}
	else if(iMode == CFB) //CFB mode, using the Chain, not using Decrypt()
		{
		SBlock crypt, chain(m_oChain);
		for(; n >= 8; n -= 8)
			{
			BytesToBlock(buf, work);
			Encrypt(chain);
			crypt = work;
			work ^= chain;
			chain = crypt;
			BlockToBytes(work, buf+=8);
			}
		}
	else //ECB mode, not using the Chain
		{
		for(; n >= 8; n -= 8)
			{
			BytesToBlock(buf, work);
			Decrypt(work);
			BlockToBytes(work, buf+=8);
			}
		}
	}

//Encrypt from Input Buffer to Output Buffer
//Returns false if n is multiple of 8
void CBlowFish::Encrypt(unsigned char* in, unsigned char* out, size_t n, int iMode)
	{
	//Check the buffer's length - should be > 0 and multiple of 8
	if((n==0)||(n%8!=0)) return;
//		throw exception("Incorrect buffer length");
	SBlock work;
	if(iMode == CBC) //CBC mode, using the Chain
		{
		SBlock chain(m_oChain);
		for(; n >= 8; n -= 8, in += 8)
			{
			BytesToBlock(in, work);
			work ^= chain;
			Encrypt(work);
			chain = work;
			BlockToBytes(work, out+=8);
			}
		}
	else if(iMode == CFB) //CFB mode, using the Chain
		{
		SBlock chain(m_oChain);
		for(; n >= 8; n -= 8, in += 8)
			{
			Encrypt(chain);
			BytesToBlock(in, work);
			work ^= chain;
			chain = work;
			BlockToBytes(work, out+=8);
			}
		}
	else //ECB mode, not using the Chain
		{
		for(; n >= 8; n -= 8, in += 8)
			{
			BytesToBlock(in, work);
			Encrypt(work);
			BlockToBytes(work, out+=8);
			}
		}
	}

//Decrypt from Input Buffer to Output Buffer
//Returns false if n is multiple of 8
void CBlowFish::Decrypt(unsigned char* in, unsigned char* out, size_t n, int iMode)
	{
	//Check the buffer's length - should be > 0 and multiple of 8
	if((n==0)||(n%8!=0)) return;
//		throw exception("Incorrect buffer length");
	SBlock work;
	if(iMode == CBC) //CBC mode, using the Chain
		{
		SBlock crypt, chain(m_oChain);
		for(; n >= 8; n -= 8, in += 8)
			{
			BytesToBlock(in, work);
			crypt = work;
			Decrypt(work);
			work ^= chain;
			chain = crypt;
			BlockToBytes(work, out+=8);
			}
		}
	else if(iMode == CFB) //CFB mode, using the Chain, not using Decrypt()
		{
		SBlock crypt, chain(m_oChain);
		for(; n >= 8; n -= 8, in += 8)
			{
			BytesToBlock(in, work);
			Encrypt(chain);
			crypt = work;
			work ^= chain;
			chain = crypt;
			BlockToBytes(work, out+=8);
			}
		}
	else //ECB mode, not using the Chain
		{
		for(; n >= 8; n -= 8, in += 8)
			{
			BytesToBlock(in, work);
			Decrypt(work);
			BlockToBytes(work, out+=8);
			}
		}
	}

void CBlowFish::Encrypt( char* strInput, unsigned char* strOutput, char* strKey,int bitCount)
	{
		CBlowFish oBlowFish((unsigned char*)strKey, bitCount);		
		int nLength = (int) strlen( strInput );
		int nDifference = nLength % 8;
		int nNewLength = nLength;

		if( nDifference	!= 0 )
			nNewLength = (nLength - nDifference) + 8;

//		char* pszDataOut = new char[ nNewLength + 1 ];
		
		oBlowFish.Encrypt((unsigned char*)strInput, (unsigned char*)strOutput, nNewLength);
		strOutput[ nNewLength ]='\0';
//		strcpy((char *) strOutput, pszDataOut);
//		delete pszDataOut;
		
	}

void CBlowFish::Decrypt( char* strInput, unsigned char* strOutput, char* strKey,int bitCount)
	{
		CBlowFish oBlowFish((unsigned char*)strKey, bitCount);		
		int nLength = (int) strlen( strInput );
		int nDifference = nLength % 8;
		int nNewLength = nLength;

		if( nDifference	!= 0 )	nNewLength = (nLength - nDifference) + 8;

//		char* pszDataOut = new char[ nNewLength ];
		oBlowFish.Decrypt((unsigned char*)strInput, (unsigned char*)strOutput, nNewLength);
		strOutput[ nNewLength ]='\0';
	}
}

void Char2Hex(const unsigned char ch, char* szHex)
	{
		unsigned char byte[2];
		byte[0] = ch/16;
		byte[1] = ch%16;
		for(int i=0; i<2; i++)
			{
			if(byte[i] >= 0 && byte[i] <= 9)
				szHex[i] = '0' + byte[i];
			else
				szHex[i] = 'A' + byte[i] - 10;
			}
		szHex[2] = 0;
	}

//Function to convert string of length 2 to unsigned char
void Hex2Char(const char* szHex, unsigned char& rch)
	{
		rch = 0;
		for(int i=0; i<2; i++)
			{
			if(*(szHex + i) >='0' && *(szHex + i) <= '9')
				rch = (rch << 4) + (*(szHex + i) - '0');
			else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
				rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
			else
				break;
			}
	}    

//Function to convert string of unsigned chars to string of chars
void CharStr2HexStr(const unsigned char* pucCharStr, std::string& strOut, int iSize)
	{
		int i;
		char szHex[3];
		
		for(i=0; i<iSize; i++)
			{
				Char2Hex(pucCharStr[i], szHex);
				strOut += szHex;
			}		
		
	}

//Function to convert string of chars to string of unsigned chars
void HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize)
	{
		int i;
		unsigned char ch;
		for(i=0; i<iSize; i++)
			{
				Hex2Char(pszHexStr+2*i, ch);
				pucCharStr[i] = ch;
			}
	}
*/