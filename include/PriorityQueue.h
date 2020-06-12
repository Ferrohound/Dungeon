#pragma once

#include <vector>

//min and max heap implement pop and insert themselves
template <typename T, typename S>
class Heap
{
	public:

		Heap<T, S>() {}
		//Heap<T, S>(std::vector<T> && d):this->data(d) {}

		void Push(T element, S size)
		{
			//insert new element at end, then percolate
			this->data.push_back( std::make_pair(element, size) );
			int i = this->data.size() - 1;

			this->PercolateUp(i);
		}

		//return smallest element in heap and delete it
		T Pop()
		{
			//remove head, move the last element to its position & percolate down
			T head = this->data[0].first;
			std::pair< T, S > tail = this->data[this->data.size() - 1];
			this->data.erase(--data.end());
			this->data[0] = tail;

			this->PercolateDown(0);

			return head;
		}

		T Peek()
		{
			return this->data[0].first;
		}

		std::size_t Size()
		{
			return this->data.size();
		}

		virtual void UpdatePriority(T element, S size)
		{
			for (int i = 0; i < this->data.size(); i++)
			{
				if(this->data[i].first == element)
				{
					if(size > this->data[i].second)
					{
						this->data[i].second = size;
						this->PercolateDown(i);
					}
					else if(size < this->data[i].second)
					{
						this->data[i].second = size;
						this->PercolateUp(i);
					}
				}
			}
		}

		bool IsEmpty()
		{
			return (this->data.size() == 0);
		}


	private:
		
		void Heapify()
		{
			this->PercolateDown(0);
		}

		virtual void PercolateUp(std::size_t index) {}

		virtual void PercolateDown(std::size_t index) {}

		//this->Swap two values
		void Swap(int a, int b)
		{
			auto tmp = this->data[a];
			this->data[a] = this->data[b];
			this->data[b] = tmp;
		}

		//get index of this->parent node
		int parent(int i) { return (i-1)/2; } 
		// to get index of this->left child of node at index i 
		int left(int i) { return (2*i + 1); } 
		// to get index of this->right child of node at index i 
		int right(int i) { return (2*i + 2); } 
		//check if index is this->valid for use on this->data
		bool valid(int i) { return (i < this->data.size());}

		std::vector< std::pair<T, S> > data;
};

template <typename T, typename S>
class MinHeap : public Heap <T, S>
{
	public: 
		void PercolateUp(std::size_t index)
		{
			//compare your value with your this->parent's
			//compare your value with your children
			if(this->data[this->parent(index)].second < this->data[index].second)
			{
				this->Swap(this->parent(index), index);
				index = this->parent(index);
				this->PercolateUp(index);
			}
		}

		void PercolateDown(std::size_t index)
		{
			int l = this->left(index);
			int r = this->right(index);

			//at least one child exists
			if( !this->valid(index) && !this->valid(r) )
				return;
			
			//if this->data is greater than this->left and this->left is either an only child or less than this->right..
			if(this->data[index].second > this->data[l].second &&
				 (!this->valid(r) || this->data[l].second < this->data[r].second))
			{
				this->Swap(l, index);
				this->PercolateDown(l);
			}

			else if(this->valid(r) && 
				this->data[index].second > this->data[r].second &&
				 this->data[r].second < this->data[l].second)
			{
				this->Swap(r, index);
				this->PercolateDown(r);
			}
		}
};

template <typename T, typename S>
class MaxHeap : public Heap <T, S>
{
	public:
		void PercolateUp(std::size_t index)
		{
			if(this->data[this->parent(index)].second > this->data[index].second)
			{
				this->Swap(this->parent(index), index);
				index = this->parent(index);
				this->PercolateUp(index);
			}
		}

		void PercolateDown(std::size_t index)
		{
			int l = this->left(index);
			int r = this->right(index);

			//at least one child exists
			if( !this->valid(index) && !this->valid(r) )
				return;
			
			//if this->data is greater than this->left and this->left is either an only child or less than this->right..
			if(this->data[index].second < this->data[l].second &&
			 (!this->valid(r) || this->data[l].second > this->data[r].second))
			{
				this->Swap(l, index);
				this->PercolateDown(l);
			}

			else if(this->valid(r) &&
			 this->data[index].second < this->data[r].second &&
			  this->data[r].second > this->data[l].second)
			{
				this->Swap(r, index);
				this->PercolateDown(r);
			}
		}

		void UpdatePriority(T element, S size)
		{
			for (int i = 0; i <this->data.size(); i++)
			{
				if(this->data[i].first == element)
				{
					if(size < this->data[i].second)
					{
						this->data[i].second = size;
						this->PercolateDown(i);
					}
					else if(size > this->data[i].second)
					{
						this->data[i].second = size;
						this->PercolateUp(i);
					}
				}
			}
		}
};