#include "filter.h"

u32 filterArray(void **inputArr, void **outputArr, const pint len, const u32 sizeOfObject, FilterList *filter, CompareFunc comparer) {
	if (inputArr == NULL || outputArr == NULL || !len || !sizeOfObject || comparer == NULL)
		return 0;

	u32 insertIndex = 0;

	if (filter->whiteListMode) {
		for (pint i = 0; i < len; i++) {
			if (containsArrayList(&filter->descriptor, inputArr[i], comparer)) {
				copyBytes((char *) inputArr, (char *) outputArr, i, insertIndex++, sizeOfObject);
			}
		}
	}

	else {
		for (pint i = 0; i < len; i++) {
			if (!containsArrayList(&filter->descriptor, inputArr[i], comparer)) {
				copyBytes((char *) inputArr, (char *) outputArr, i, insertIndex++, sizeOfObject);
			}
		}
	}

	return insertIndex;
}

u32 filterArrayList(ArrayList *inputList, ArrayList *outputList, FilterList *filter, CompareFunc comparer) {
	if (inputList == NULL || !inputList->len || outputList == NULL || filter == NULL || comparer == NULL)
		return 0;

	if (filter->whiteListMode) {
		for (u32 i = 0; i < inputList->len; i++) {
			void *currentObj = getArrayList(inputList, i);

			if (containsArrayList(&filter->descriptor, currentObj, comparer)) {
				// copyBytes((char *) inputArr, (char *) outputArr, i, insertIndex++, sizeOfObject);
				addArrayList(outputList, currentObj);
			}
		}
	}

	else {
		for (u32 i = 0; i < inputList->len; i++) {
			void *currentObj = getArrayList(inputList, i);

			if (!containsArrayList(&filter->descriptor, currentObj, comparer)) {
				// copyBytes((char *) inputArr, (char *) outputArr, i, insertIndex++, sizeOfObject);
				addArrayList(outputList, currentObj);
			}
		}
	}

	return outputList->len;
}