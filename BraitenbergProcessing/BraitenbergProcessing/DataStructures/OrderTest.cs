using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BraitenbergProcessing.DataStructures
{
    public class Order
    {
        public string Receipt { get; set; }
        public DateTime Date { get; set; }
        public Customer Customer { get; set; }
        public List<OrderItem> Items { get; set; }

        [YamlAlias("bill-to")]
        public Address BillTo { get; set; }

        [YamlAlias("ship-to")]
        public Address ShipTo { get; set; }

        public string SpecialDelivery { get; set; }
    }

    public class Customer
    {
        public string Given { get; set; }
        public string Family { get; set; }
    }

    public class OrderItem
    {
        [YamlAlias("part_no")]
        public string PartNo { get; set; }
        public string Descrip { get; set; }
        public decimal Price { get; set; }
        public int Quantity { get; set; }
    }

    public class Address
    {
        public string Street { get; set; }
        public string City { get; set; }
        public string State { get; set; }
    }
}
