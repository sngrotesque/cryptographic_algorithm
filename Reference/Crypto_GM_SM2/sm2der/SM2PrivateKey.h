/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "Sm2derSimplfied"
 * 	found in "module.asn1"
 */

#ifndef	_SM2PrivateKey_H_
#define	_SM2PrivateKey_H_


#include <asn_application.h>

/* Including external dependencies */
#include <INTEGER.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SM2PrivateKey */
typedef INTEGER_t	 SM2PrivateKey_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SM2PrivateKey;
asn_struct_free_f SM2PrivateKey_free;
asn_struct_print_f SM2PrivateKey_print;
asn_constr_check_f SM2PrivateKey_constraint;
ber_type_decoder_f SM2PrivateKey_decode_ber;
der_type_encoder_f SM2PrivateKey_encode_der;
xer_type_decoder_f SM2PrivateKey_decode_xer;
xer_type_encoder_f SM2PrivateKey_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _SM2PrivateKey_H_ */
