#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>		//malloc/realloc����
#include <math.h>
#include <string.h>
#include "liner_list_L.h"	//��ʽ����

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status InitList(LinkList* L)
{
	/* ����ͷ���ռ䣬��ֵ��ͷָ�� */
	*L = (LNode*)malloc(sizeof(LNode));
	if (*L == NULL)
		exit(LOVERFLOW);

	(*L)->next = NULL;
	return OK;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ɾ������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status DestroyList(LinkList* L)
{
	LinkList q, p = *L; //ָ����Ԫ
#if defined (ELEMTYPE_IS_CHAR_P) || defined (ELEMTYPE_IS_STRUCT_STUDENT_P)

	/* �����ͷŶ����ռ� */
	while (p->next) {
		q = p->next;
		free(p->data);
		p = q;
	}

#endif
	p = *L;
	/* ��������(��ͷ���)�����ͷ� */
	while (p) {    //������Ϊ�գ���ѭ����ִ��
		q = p->next; //ץס�������һ�����
		free(p);
		p = q;
	}

	*L = NULL;	//ͷָ����NULL
	return OK;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ͷ��㣩
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status ClearList(LinkList* L)
{
	LinkList q, p = (*L)->next;

#if defined (ELEMTYPE_IS_CHAR_P) || defined (ELEMTYPE_IS_STRUCT_STUDENT_P)

	/* �����ͷŶ����ռ� */
	while (p) {
		q = p->next;
		free(p->data);
		p = q;
	}

#endif
	p = (*L)->next;
	/* ����Ԫ��㿪ʼ�����ͷ� */
	while (p) {
		q = p->next;   //ץס�������һ�����
		free(p);
		p = q;
	}

	(*L)->next = NULL; //ͷ����next����NULL
	return OK;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ж��Ƿ�Ϊ�ձ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status ListEmpty(LinkList L)
{
	/* �ж�ͷ����next�򼴿� */
	if (L->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ĳ���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int ListLength(LinkList L)
{
	LinkList p = L->next; //ָ����Ԫ���
	int len = 0;

	/* ѭ�������������м��� */
	while (p) {
		p = p->next;
		len++;
	}

	return len;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡ����Ԫ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status GetElem(LinkList L, int i, ElemType* e)
{
	LinkList p = L->next;	//ָ����Ԫ���
	int    pos = 1;		//��ʼλ��Ϊ1

	/* ����ΪNULL �� δ����i��Ԫ�� */
	while (p != NULL && pos < i) {
		p = p->next;
		pos++;
	}

	if (!p || pos > i)
		return ERROR;
	/* ѭ���Ƚ��������� */
#if defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	strcpy(*e, p->data);	//�±��0��ʼ����i��ʵ����elem[i-1]��
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(e, &(p->data), sizeof(ElemType)); //�±��0��ʼ����i��ʵ����elem[i-1]��
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	memcpy(*e, p->data, sizeof(ET)); //�±��0��ʼ����i��ʵ����elem[i-1]��
#else	//int��doubleֱ�Ӹ�ֵ
	* e = p->data;	//�±��0��ʼ����i��ʵ����elem[i-1]��
#endif
	return OK;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ҷ���ָ��������Ԫ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int LocateElem(LinkList L, ElemType e, Status(*compare)(ElemType e1, ElemType e2))
{
	LinkList p = L->next;	//��Ԫ���
	int    pos = 1;		//��ʼλ��

	/* ѭ����������  */
	while (p && (*compare)(e, p->data) == FALSE) {
		p = p->next;
		pos++;
	}

	return p ? pos : 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ҷ���ָ��������Ԫ�ص�ǰ��Ԫ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status PriorElem(LinkList L, ElemType cur_e, ElemType* pre_e)
{
	LinkList p = L->next;	//ָ����Ԫ���
		/* ѭ���Ƚ��������� */
#ifdef ELEMTYPE_IS_DOUBLE
	while (p->next && fabs(p->next->data - cur_e) >= 1e-6) {
#elif defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	while (p->next && strcmp(p->next->data, cur_e)) {
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	while (p->next && p->next->data.num != cur_e.num) {
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	while (p->next && p->next->data->num != cur_e->num) {
#else	//ȱʡ����int����
	while (p->next && p->next->data != cur_e) {
#endif
		p = p->next;
	}

	if (p->next == NULL) //δ�ҵ�
		return ERROR;

#if defined (ELEMTYPE_IS_CHAR_ARRAY) || defined(ELEMTYPE_IS_CHAR_P)
	strcpy(*pre_e, p->data);	//ȡǰ��Ԫ�ص�ֵ
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(pre_e, &(p->data), sizeof(ElemType));	//ȡǰ��Ԫ�ص�ֵ
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	memcpy(*pre_e, p->data, sizeof(ET));	//ȡǰ��Ԫ�ص�ֵ
#else	//int��doubleֱ�Ӹ�ֵ
	*pre_e = p->data;	//ȡǰ��Ԫ�ص�ֵ
#endif
	return OK;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ҷ���ָ��������Ԫ�صĺ��Ԫ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status NextElem(LinkList L, ElemType cur_e, ElemType * next_e)
{
	LinkList p = L->next;  //��Ԫ���

#ifdef ELEMTYPE_IS_DOUBLE
	while (p->next && fabs(p->data - cur_e) >= 1e-6) {
#elif defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	while (p->next && strcmp(p->data, cur_e)) {
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	while (p->next && p->data.num != cur_e.num) {
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	while (p->next && p->data->num != cur_e->num) {
#else	//ȱʡ����int����
	while (p->next && p->data != cur_e) {
#endif
		p = p->next;
	}

	if (p->next == NULL)
		return ERROR;


#if defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	strcpy(*next_e, p->next->data);	//ȡ���Ԫ�ص�ֵ
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(next_e, &(p->next->data), sizeof(ElemType));	//ȡ���Ԫ�ص�ֵ
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	memcpy(*next_e, p->next->data, sizeof(ET));	//ȡ���Ԫ�ص�ֵ
#else	//int��doubleֱ�Ӹ�ֵ
	* next_e = p->next->data;	//ȡ���Ԫ�ص�ֵ
#endif
	return OK;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ָ��λ��ǰ����һ����Ԫ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status ListInsert(LinkList * L, int i, ElemType e)
{
	LinkList s, p = *L;	//pָ��ͷ���
	int      pos = 0;

	/* Ѱ�ҵ�i-1����� */
	while (p && pos < i - 1) {
		p = p->next;
		pos++;
	}

	if (p == NULL || pos > i - 1)  //iֵ�Ƿ��򷵻�
		return ERROR;

	//ִ�е��˱�ʾ�ҵ�ָ��λ�ã�pָ���i-1�����

	s = (LinkList)malloc(sizeof(LNode));
	if (s == NULL)
		return LOVERFLOW;

#if defined (ELEMTYPE_IS_CHAR_ARRAY)
	strcpy((s->data), e);
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(&s->data, &e, sizeof(ElemType));	//������strcpy
#elif defined(ELEMTYPE_IS_CHAR_P)
	s->data = (ElemType)malloc((strlen(e) + 1) * sizeof(char));
	if (s->data == NULL)
		return LOVERFLOW;

	strcpy(s->data, e);
#elif defined(ELEMTYPE_IS_STRUCT_STUDENT_P)
	s->data = (ElemType)malloc(sizeof(ET));
	if (s->data == NULL)
		return LOVERFLOW;

	memcpy(s->data, e, sizeof(ET));

#else	//int��double��charָ�롢struct studentָ�붼��ֱ�Ӹ�ֵ
	s->data = e;
#endif 	//�½��������ֵ
	s->next = p->next;	//�½���next�ǵ�i��
	p->next = s;	//��i-1����next���½��

	return OK;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ɾ��ָ��λ�õ�Ԫ�أ�������ɾ��Ԫ�ص�ֵ����e�з���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status ListDelete(LinkList * L, int i, ElemType * e)
{
	LinkList q, p = *L;	//pָ��ͷ���
	int      pos = 0;

	/* Ѱ�ҵ�i����㣨p->next�ǵ�i����㣩 */
	while (p->next && pos < i - 1) {
		p = p->next;
		pos++;
	}

	if (p->next == NULL || pos > i - 1)	//iֵ�Ƿ��򷵻�
		return ERROR;

	//ִ�е��˱�ʾ�ҵ��˵�i����㣬��ʱpָ���i-1�����

	q = p->next;       //qָ���i�����
	p->next = q->next; //��i-1������next��ָ���i+1��
#if defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	strcpy(*e, q->data); 				//ȡ��i��Ԫ�ص�ֵ����e��
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(e, &(q->data), sizeof(ElemType));	//ȡ��i��Ԫ�ص�ֵ����e��
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	memcpy(*e, q->data, sizeof(ET));		//ȡ��i��Ԫ�ص�ֵ����e��
#else	//int��doubleֱ�Ӹ�ֵ
	* e = q->data; 				//ȡ��i��Ԫ�ص�ֵ����e��
#endif
	free(q);           //�ͷŵ�i�����

	return OK;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
Status ListTraverse(LinkList L, Status(*visit)(ElemType e))
{
	extern int line_count; //��main�ж���Ĵ�ӡ���м����������㷨�޹أ�
	LinkList p = L->next;

	line_count = 0;		//�������ָ���ʼֵ�����㷨�޹أ�
	while (p && (*visit)(p->data) == TRUE)
		p = p->next;

	if (p)
		return ERROR;

	printf("\n");//����ӡһ�����У�ֻ��Ϊ�˺ÿ������㷨�޹�
	return OK;
}
