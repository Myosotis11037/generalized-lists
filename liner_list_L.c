#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>		//malloc/realloc函数
#include <math.h>
#include <string.h>
#include "liner_list_L.h"	//形式定义

/***************************************************************************
  函数名称：
  功    能：初始化链表
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status InitList(LinkList* L)
{
	/* 申请头结点空间，赋值给头指针 */
	*L = (LNode*)malloc(sizeof(LNode));
	if (*L == NULL)
		exit(LOVERFLOW);

	(*L)->next = NULL;
	return OK;
}

/***************************************************************************
  函数名称：
  功    能：删除链表
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status DestroyList(LinkList* L)
{
	LinkList q, p = *L; //指向首元
#if defined (ELEMTYPE_IS_CHAR_P) || defined (ELEMTYPE_IS_STRUCT_STUDENT_P)

	/* 首先释放二级空间 */
	while (p->next) {
		q = p->next;
		free(p->data);
		p = q;
	}

#endif
	p = *L;
	/* 整个链表(含头结点)依次释放 */
	while (p) {    //若链表为空，则循环不执行
		q = p->next; //抓住链表的下一个结点
		free(p);
		p = q;
	}

	*L = NULL;	//头指针置NULL
	return OK;
}

/***************************************************************************
  函数名称：
  功    能：清除链表（保留头结点）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status ClearList(LinkList* L)
{
	LinkList q, p = (*L)->next;

#if defined (ELEMTYPE_IS_CHAR_P) || defined (ELEMTYPE_IS_STRUCT_STUDENT_P)

	/* 首先释放二级空间 */
	while (p) {
		q = p->next;
		free(p->data);
		p = q;
	}

#endif
	p = (*L)->next;
	/* 从首元结点开始依次释放 */
	while (p) {
		q = p->next;   //抓住链表的下一个结点
		free(p);
		p = q;
	}

	(*L)->next = NULL; //头结点的next域置NULL
	return OK;
}

/***************************************************************************
  函数名称：
  功    能：判断是否为空表
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status ListEmpty(LinkList L)
{
	/* 判断头结点的next域即可 */
	if (L->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/***************************************************************************
  函数名称：
  功    能：求表的长度
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int ListLength(LinkList L)
{
	LinkList p = L->next; //指向首元结点
	int len = 0;

	/* 循环整个链表，进行计数 */
	while (p) {
		p = p->next;
		len++;
	}

	return len;
}

/***************************************************************************
  函数名称：
  功    能：取表中元素
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status GetElem(LinkList L, int i, ElemType* e)
{
	LinkList p = L->next;	//指向首元结点
	int    pos = 1;		//初始位置为1

	/* 链表不为NULL 且 未到第i个元素 */
	while (p != NULL && pos < i) {
		p = p->next;
		pos++;
	}

	if (!p || pos > i)
		return ERROR;
	/* 循环比较整个链表 */
#if defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	strcpy(*e, p->data);	//下标从0开始，第i个实际在elem[i-1]中
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(e, &(p->data), sizeof(ElemType)); //下标从0开始，第i个实际在elem[i-1]中
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	memcpy(*e, p->data, sizeof(ET)); //下标从0开始，第i个实际在elem[i-1]中
#else	//int和double直接赋值
	* e = p->data;	//下标从0开始，第i个实际在elem[i-1]中
#endif
	return OK;
}

/***************************************************************************
  函数名称：
  功    能：查找符合指定条件的元素
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int LocateElem(LinkList L, ElemType e, Status(*compare)(ElemType e1, ElemType e2))
{
	LinkList p = L->next;	//首元结点
	int    pos = 1;		//初始位置

	/* 循环整个链表  */
	while (p && (*compare)(e, p->data) == FALSE) {
		p = p->next;
		pos++;
	}

	return p ? pos : 0;
}

/***************************************************************************
  函数名称：
  功    能：查找符合指定条件的元素的前驱元素
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status PriorElem(LinkList L, ElemType cur_e, ElemType* pre_e)
{
	LinkList p = L->next;	//指向首元结点
		/* 循环比较整个链表 */
#ifdef ELEMTYPE_IS_DOUBLE
	while (p->next && fabs(p->next->data - cur_e) >= 1e-6) {
#elif defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	while (p->next && strcmp(p->next->data, cur_e)) {
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	while (p->next && p->next->data.num != cur_e.num) {
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	while (p->next && p->next->data->num != cur_e->num) {
#else	//缺省当做int处理
	while (p->next && p->next->data != cur_e) {
#endif
		p = p->next;
	}

	if (p->next == NULL) //未找到
		return ERROR;

#if defined (ELEMTYPE_IS_CHAR_ARRAY) || defined(ELEMTYPE_IS_CHAR_P)
	strcpy(*pre_e, p->data);	//取前驱元素的值
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(pre_e, &(p->data), sizeof(ElemType));	//取前驱元素的值
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	memcpy(*pre_e, p->data, sizeof(ET));	//取前驱元素的值
#else	//int和double直接赋值
	*pre_e = p->data;	//取前驱元素的值
#endif
	return OK;
}

/***************************************************************************
  函数名称：
  功    能：查找符合指定条件的元素的后继元素
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status NextElem(LinkList L, ElemType cur_e, ElemType * next_e)
{
	LinkList p = L->next;  //首元结点

#ifdef ELEMTYPE_IS_DOUBLE
	while (p->next && fabs(p->data - cur_e) >= 1e-6) {
#elif defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	while (p->next && strcmp(p->data, cur_e)) {
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	while (p->next && p->data.num != cur_e.num) {
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	while (p->next && p->data->num != cur_e->num) {
#else	//缺省当做int处理
	while (p->next && p->data != cur_e) {
#endif
		p = p->next;
	}

	if (p->next == NULL)
		return ERROR;


#if defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	strcpy(*next_e, p->next->data);	//取后继元素的值
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(next_e, &(p->next->data), sizeof(ElemType));	//取后继元素的值
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	memcpy(*next_e, p->next->data, sizeof(ET));	//取后继元素的值
#else	//int和double直接赋值
	* next_e = p->next->data;	//取后继元素的值
#endif
	return OK;
}

/***************************************************************************
  函数名称：
  功    能：在指定位置前插入一个新元素
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status ListInsert(LinkList * L, int i, ElemType e)
{
	LinkList s, p = *L;	//p指向头结点
	int      pos = 0;

	/* 寻找第i-1个结点 */
	while (p && pos < i - 1) {
		p = p->next;
		pos++;
	}

	if (p == NULL || pos > i - 1)  //i值非法则返回
		return ERROR;

	//执行到此表示找到指定位置，p指向第i-1个结点

	s = (LinkList)malloc(sizeof(LNode));
	if (s == NULL)
		return LOVERFLOW;

#if defined (ELEMTYPE_IS_CHAR_ARRAY)
	strcpy((s->data), e);
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(&s->data, &e, sizeof(ElemType));	//不能用strcpy
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

#else	//int、double、char指针、struct student指针都是直接赋值
	s->data = e;
#endif 	//新结点数据域赋值
	s->next = p->next;	//新结点的next是第i个
	p->next = s;	//第i-1个的next是新结点

	return OK;
}

/***************************************************************************
  函数名称：
  功    能：删除指定位置的元素，并将被删除元素的值放入e中返回
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status ListDelete(LinkList * L, int i, ElemType * e)
{
	LinkList q, p = *L;	//p指向头结点
	int      pos = 0;

	/* 寻找第i个结点（p->next是第i个结点） */
	while (p->next && pos < i - 1) {
		p = p->next;
		pos++;
	}

	if (p->next == NULL || pos > i - 1)	//i值非法则返回
		return ERROR;

	//执行到此表示找到了第i个结点，此时p指向第i-1个结点

	q = p->next;       //q指向第i个结点
	p->next = q->next; //第i-1个结点的next域指向第i+1个
#if defined (ELEMTYPE_IS_CHAR_ARRAY) || defined (ELEMTYPE_IS_CHAR_P)
	strcpy(*e, q->data); 				//取第i个元素的值放入e中
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT)
	memcpy(e, &(q->data), sizeof(ElemType));	//取第i个元素的值放入e中
#elif defined (ELEMTYPE_IS_STRUCT_STUDENT_P)
	memcpy(*e, q->data, sizeof(ET));		//取第i个元素的值放入e中
#else	//int和double直接赋值
	* e = q->data; 				//取第i个元素的值放入e中
#endif
	free(q);           //释放第i个结点

	return OK;
}

/***************************************************************************
  函数名称：
  功    能：遍历链表
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
Status ListTraverse(LinkList L, Status(*visit)(ElemType e))
{
	extern int line_count; //在main中定义的打印换行计数器（与算法无关）
	LinkList p = L->next;

	line_count = 0;		//计数器恢复初始值（与算法无关）
	while (p && (*visit)(p->data) == TRUE)
		p = p->next;

	if (p)
		return ERROR;

	printf("\n");//最后打印一个换行，只是为了好看，与算法无关
	return OK;
}
